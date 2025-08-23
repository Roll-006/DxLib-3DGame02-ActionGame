#include "hit_points.hpp"

HitPoints::HitPoints() :
	m_current_hit_points(0.0f),
	m_max_hit_points	(0.0f)
{

}

HitPoints::~HitPoints()
{

}

void HitPoints::Recover(const float recover_points)
{
	math::Increase(m_current_hit_points, recover_points, m_max_hit_points, false);
}

void HitPoints::OnDamage(const float damage, const BodyPartKind on_damage_part)
{

}

float HitPoints::GetCurrentHitPoints(const BodyPartKind body_part_kind) const
{
	return m_current_hit_points.at(body_part_kind);
}
