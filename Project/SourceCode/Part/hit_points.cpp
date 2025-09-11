#include "hit_points.hpp"

HitPoints::HitPoints(const float max_hit_points, const float max_current_hit_points) :
	m_current_hit_points	(max_current_hit_points),
	m_current_max_hit_points(max_current_hit_points),
	m_max_hit_points		(max_hit_points)
{

}

HitPoints::~HitPoints()
{

}

void HitPoints::Recover(const float recover_points)
{
	math::Increase(m_current_hit_points, recover_points, m_current_max_hit_points, false);
}

void HitPoints::OnDamage(const float damage)
{
	math::Decrease(m_current_hit_points, damage, 0.0f);
}

void HitPoints::SetCurrentMaxHitPoints(const float current_max_hit_points)
{
	m_current_max_hit_points = current_max_hit_points;
	if (m_current_max_hit_points > m_max_hit_points)
	{
		m_current_max_hit_points = m_max_hit_points;
	}
}
