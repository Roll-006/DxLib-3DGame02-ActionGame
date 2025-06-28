#include "gun_base.hpp"

VECTOR GunBase::GetMuzzlePos() const
{
	const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos = m_transform->GetPos(CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_muzzle_correct_pos, world_m);
}

VECTOR GunBase::GetEjectionPortPos() const
{
	const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos = m_transform->GetPos(CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_ejection_port_correct_pos, world_m);
}

void GunBase::CalcRayPos()
{
	if (!m_is_aiming) { return; }

	// 光線の始点を計算
	Segment s1 = Segment(m_point_on_ray_line, m_point_on_ray_line + m_aim_dir);
	Segment s2 = Segment(m_point_on_ray_line, GetMuzzlePos());
	VECTOR  v1 = s1.GetEndPos() - s1.GetBeginPos();
	VECTOR  v2 = s2.GetEndPos() - s2.GetBeginPos();
	VECTOR  h  = math::GetProjectionVector(v2, v1);
	VECTOR  ray_begin_pos = s1.GetBeginPos() + h;

	// 光線の座標を更新
	auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	ray->SetBeginPos(ray_begin_pos, true);
	ray->SetEndPos  (ray->GetBeginPos() + m_aim_dir * m_range, true);
}

void GunBase::JudgeShot()
{
	if (!m_is_aiming)
	{
		m_shot_timer = 0.0f;
		m_is_shot    = false;
		return;
	}

	// エイミング中のみ発射タイマーを増加
	math::IncreaseLoop(m_shot_timer, FPS::GetDeltaTime(), m_shot_interval_time, true);
	m_is_shot = m_shot_timer == 0.0f ? true : false;
}
