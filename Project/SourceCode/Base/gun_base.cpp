#include "gun_base.hpp"
#include "../Command/command_handler.hpp"

GunBase::GunBase(const std::string& name, const GunKind gun_kind, const HolsterKind holster_kind) :
	WeaponBase						(name, WeaponKind::kGun, holster_kind),
	m_magazine						(nullptr),
	m_cross_hair_shape				(nullptr),
	m_muzzle_transform				(std::make_shared<Transform>()),
	m_ejection_port_transform		(nullptr),
	m_load_transform				(std::make_shared<Transform>()),
	m_aim_dir						(v3d::GetZeroV()),
	m_target_pos					(v3d::GetZeroV()),
	m_point_on_ray					(v3d::GetZeroV()),
	m_current_remaining_bullet_num	(0),
	max_remaining_bullet_num		(0),
	scope_scale						(0.0f),
	range							(0.0f),
	initial_velocity				(0.0f),
	deceleration					(0.0f),
	recoil_data						(RecoilData()),
	shot_interval_time				(0.0f),
	m_shot_timer					(0.0f),
	m_on_pull_trigger				(false),
	gun_kind						(gun_kind)
{

}

void GunBase::TrackOwnerHand()
{
	if (!m_owner_modeler) { return; }
	m_owner_modeler->ApplyMatrix();

	// アタッチする部位の行列情報を取り出す
	const auto owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.RIGHT_HAND);
	const auto owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(hold_offset_angle);
	const auto result_mat		= offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(hold_offset_pos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, hold_offset_scale);

	CalcTransforms();

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->LateUpdate();
}

void GunBase::TrackOwnerHolster()
{
	if (!m_owner_modeler) { return; }
	m_owner_modeler->ApplyMatrix();

	int owner_attach_frame_num = -1;

	switch (holster_kind)
	{
	case HolsterKind::kKnife:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.SPINE_2);
		break;

	case HolsterKind::kHandgun:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.RIGHT_UP_LEG);
		break;

	case HolsterKind::kRifle:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.SPINE_2);
		break;

	case HolsterKind::kGrenade:
		owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), FramePath.RIGHT_UP_LEG);
		break;
	}

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);
	const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(attach_offset_angle);
	const auto result_mat = offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(attach_offset_pos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, attach_offset_scale);

	CalcTransforms();

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->LateUpdate();
}

void GunBase::OnShot()
{
	RifleCartridgeManager::GetInstance()->SearchValidRifleCartidge(*this);
	--m_current_remaining_bullet_num;

	const auto time_scale = m_owner_name == ObjName.PLAYER ? TimeScaleLayerKind::kPlayer : TimeScaleLayerKind::kWorld;
	EventSystem::GetInstance()->Publish(WeaponShotEvent(gun_kind, m_owner_name, m_muzzle_transform, m_ejection_port_transform, TimeScaleLayerKind::kPlayer));
}

int GunBase::OnReload(const int have_bullets)
{
	int remaining_bullets_num = have_bullets;

	// 既に最大値の場合はリロードさせない
	if (m_current_remaining_bullet_num >= max_remaining_bullet_num) { return remaining_bullets_num; }

	// 不足分の計算
	const int shortage_num = max_remaining_bullet_num - m_current_remaining_bullet_num;

	// 可能な数だけリロードし、所持している弾丸を減少させる
	if (remaining_bullets_num < shortage_num)
	{
		remaining_bullets_num = 0;
		m_current_remaining_bullet_num += have_bullets;
	}
	else
	{
		remaining_bullets_num -= shortage_num;
		m_current_remaining_bullet_num += shortage_num;
	}

	return remaining_bullets_num;
}


#pragma region Getter
VECTOR GunBase::GetShotDir() const
{
	return v3d::GetNormalizedV(m_target_pos - GetFirstShotPos());
}

VECTOR GunBase::GetFirstShotPos() const
{
	// 操作キャラの場合は銃口からカメラforwardに投影
	// 非操作キャラの場合はそのまま銃口
	Segment s1 = Segment(m_point_on_ray, m_point_on_ray + m_aim_dir);
	Segment s2 = Segment(m_point_on_ray, GetMuzzleTransform()->GetPos(CoordinateKind::kWorld));
	VECTOR  v1 = s1.GetEndPos() - s1.GetBeginPos();
	VECTOR  v2 = s2.GetEndPos() - s2.GetBeginPos();
	VECTOR  h  = math::GetProjectionVector(v2, v1);

	return s1.GetBeginPos() + h;
}

bool GunBase::IsShot() const
{
	if (m_shot_timer == 0.0f && m_on_pull_trigger)
	{
		if (m_current_remaining_bullet_num > 0)
		{
			return true;
		}

		if (!CommandHandler::GetInstance()->IsExecute(CommandKind::kPullTrigger, TimeKind::kPrev))
		{
			const auto time_scale = m_owner_name == ObjName.PLAYER ? TimeScaleLayerKind::kPlayer : TimeScaleLayerKind::kWorld;
			EventSystem::GetInstance()->Publish(EmptyAmmoEvent(m_transform->GetPos(CoordinateKind::kWorld), time_scale));
		}
	}
	return false;
}

bool GunBase::CanReload() const
{
	return m_current_remaining_bullet_num < max_remaining_bullet_num;
}
#pragma endregion


void GunBase::CalcShotTimer()
{
	// TODO : 連射が可能なため後に仕様変更
	if (m_on_pull_trigger)
	{
		math::Increase(m_shot_timer, GetDeltaTime(), shot_interval_time, true);
	}
	else
	{
		m_shot_timer = 0.0f;
	}
}

void GunBase::CalcTransform(std::shared_ptr<Transform>& transform, const VECTOR& offset)
{
	if (!transform) { transform = std::make_shared<Transform>(); }

	transform->SetMatrix(CoordinateKind::kWorld, m_transform->GetMatrix(CoordinateKind::kWorld));

	const auto world_m		= m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos	= m_transform->GetPos(CoordinateKind::kLocal);
	const auto pos			= VTransformSR(offset, world_m);

	transform->SetPos(CoordinateKind::kWorld, local_pos + pos);
}
