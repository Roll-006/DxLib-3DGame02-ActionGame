#include "weapon_base.hpp"

WeaponBase::WeaponBase(const std::string& name, const WeaponKind weapon_kind, const HolsterKind holster_kind) :
	PhysicalObjBase	(name, ObjTag.WEAPON, MassKind::kLight),
	m_modeler		(nullptr),
	m_owner_modeler	(nullptr),
	m_offset_pos	(v3d::GetZeroV()),
	m_offset_angle	(v3d::GetZeroV()),
	m_offset_scale	(v3d::GetZeroV()),
	m_item_kind		(ItemKind::kWeapon),
	m_weapon_kind	(weapon_kind),
	m_holster_kind	(holster_kind)
{
	
}

void WeaponBase::TrackOwner()
{
	if (!m_owner_modeler) { return; }

	m_owner_modeler->ApplyMatrix();

	// アタッチする部位の行列情報を取り出す
	const int    owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.RIGHT_HAND);
	const MATRIX owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	MATRIX offset_angle_mat = MGetIdent();
	CreateRotationXYZMatrix(&offset_angle_mat, m_offset_angle.x, m_offset_angle.y, m_offset_angle.z);
	const MATRIX result_mat = offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos   (CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(m_offset_pos, result_mat));
	m_transform->SetScale (CoordinateKind::kWorld, m_offset_scale);
}
