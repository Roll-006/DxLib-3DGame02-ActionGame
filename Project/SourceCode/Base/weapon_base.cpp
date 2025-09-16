#include "weapon_base.hpp"

WeaponBase::WeaponBase(const std::string& name, const WeaponKind weapon_kind, const HolsterKind holster_kind) :
	PhysicalObjBase			(name, ObjTag.WEAPON, MassKind::kLight),
	m_modeler				(nullptr),
	m_owner_modeler			(nullptr),
	m_owner_name			(""),
	m_power					(0.0f),
	m_hold_offset_pos		(v3d::GetZeroV()),
	m_hold_offset_angle		(v3d::GetZeroV()),
	m_hold_offset_scale		(v3d::GetZeroV()),
	m_attach_offset_pos		(v3d::GetZeroV()),
	m_attach_offset_angle	(v3d::GetZeroV()),
	m_attach_offset_scale	(v3d::GetZeroV()),
	m_item_kind				(ItemKind::kWeapon),
	m_weapon_kind			(weapon_kind),
	m_holster_kind			(holster_kind)
{
	
}

void WeaponBase::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void WeaponBase::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		 (obj_handle);
}

/// @brief 装備する持ち主をアタッチする
void WeaponBase::AttachOwner(const std::shared_ptr<Modeler> owner_modeler, const std::string& owner_name)
{
	if (!m_owner_modeler)
	{
		m_owner_modeler = owner_modeler;
		m_owner_name	= owner_name;
	}
}
/// @brief 装備していた持ち主をデタッチする
void WeaponBase::DetachOwner()
{
	if (m_owner_modeler)
	{
		m_owner_modeler = nullptr;
		m_owner_name	= "";
	}
}

void WeaponBase::TrackOwnerHand()
{
	if (!m_owner_modeler) { return; }
	m_owner_modeler->ApplyMatrix();

	// アタッチする部位の行列情報を取り出す
	const auto owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.RIGHT_HAND);
	const auto owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(m_hold_offset_angle);
	const auto result_mat		= offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(m_hold_offset_pos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, m_hold_offset_scale);
}

void WeaponBase::TrackOwnerHolster()
{
	if (!m_owner_modeler) { return; }
	m_owner_modeler->ApplyMatrix();

	int owner_attach_frame_num = -1;

	switch (m_holster_kind)
	{
	case HolsterKind::kKnife:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.SPINE_2);
		break;

	case HolsterKind::kHandgun:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.RIGHT_UP_LEG);
		break;

	case HolsterKind::kRifle:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.SPINE_2);
		break;

	case HolsterKind::kGrenade:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.RIGHT_UP_LEG);
		break;
	}


	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto owner_attach_frame_mat	= MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);
	const auto offset_angle_mat			= math::ConvertEulerAnglesToXYZRotMatrix(m_attach_offset_angle);
	const auto result_mat				= offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(m_attach_offset_pos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, m_attach_offset_scale);
}

float WeaponBase::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

void WeaponBase::SetOffset(
	const VECTOR& hold_pos,
	const VECTOR& hold_angle,
	const VECTOR& hold_scale,
	const VECTOR& attach_pos,
	const VECTOR& attach_angle,
	const VECTOR& attach_scale)
{
	m_hold_offset_pos		= hold_pos;
	m_hold_offset_angle		= hold_angle;
	m_hold_offset_scale		= hold_scale;
	m_attach_offset_pos		= attach_pos;
	m_attach_offset_angle	= attach_angle;
	m_attach_offset_scale	= attach_scale;
}

void WeaponBase::SetOffset(
	const VECTOR& hold_pos,
	const VECTOR& hold_angle,
	const float   hold_scale,
	const VECTOR& attach_pos,
	const VECTOR& attach_angle,
	const float   attach_scale)
{
	m_hold_offset_pos		= hold_pos;
	m_hold_offset_angle		= hold_angle;
	m_hold_offset_scale		= VGet(hold_scale, hold_scale, hold_scale);
	m_attach_offset_pos		= attach_pos;
	m_attach_offset_angle	= attach_angle;
	m_attach_offset_scale	= VGet(attach_scale, attach_scale, attach_scale);
}
