#include "gun_base.hpp"

GunBase::GunBase(const std::string& name, const GunKind gun_kind, const std::string& file_path) :
	WeaponBase					(name, file_path),
	m_aim_dir					(v3d::GetZeroV()),
	m_muzzle_correct_pos		(v3d::GetZeroV()),
	m_ejection_port_correct_pos	(v3d::GetZeroV()),
	m_point_on_ray_line			(v3d::GetZeroV()),
	m_scope_scale				(0.0f),
	m_range						(0.0f),
	m_initial_velocity			(0.0f),
	m_shot_interval_time		(0.0f),
	m_shot_timer				(0.0f),
	m_is_shot					(false),
	m_is_pull_trigger			(false),
	m_gun_kind					(gun_kind)
{

}

VECTOR GunBase::GetMuzzlePos() const
{
	const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos = m_transform->GetPos   (CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_muzzle_correct_pos, world_m);
}

VECTOR GunBase::GetEjectionPortPos() const
{
	const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos = m_transform->GetPos   (CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_ejection_port_correct_pos, world_m);
}

VECTOR GunBase::GetFirstShotPos() const
{
	// 操作キャラの場合は銃口からカメラforwardに投影
	// 非操作キャラの場合はそのまま銃口
	Segment s1 = Segment(m_point_on_ray_line, m_point_on_ray_line + m_aim_dir);
	Segment s2 = Segment(m_point_on_ray_line, GetMuzzlePos());
	VECTOR  v1 = s1.GetEndPos() - s1.GetBeginPos();
	VECTOR  v2 = s2.GetEndPos() - s2.GetBeginPos();
	VECTOR  h  = math::GetProjectionVector(v2, v1);

	return s1.GetBeginPos() + h;
}

void GunBase::Shot()
{
	if (!m_is_pull_trigger)
	{
		m_shot_timer = 0.0f;
		m_is_shot    = false;
		return;
	}

	// エイミング中のみ発射タイマーを増加
	math::IncreaseLoop(m_shot_timer, FPS::GetDeltaTime(), m_shot_interval_time, true);
	m_is_shot = m_shot_timer == 0.0f ? true : false;

	// 発射呼び出し
	if (m_is_shot)
	{
		RifleCartridgeManager::GetInstance()->Shot(*this);
	}
}
