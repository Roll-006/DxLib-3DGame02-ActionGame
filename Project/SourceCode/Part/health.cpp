#include "health.hpp"

Health::Health(const float max_health, const float max_current_health) :
	m_current_health	(max_current_health),
	m_current_max_health(max_current_health),
	m_max_health		(max_health)
{

}

Health::~Health()
{

}

void Health::Recover(const float recover_points)
{
	math::Increase(m_current_health, recover_points, m_current_max_health, false);
}

void Health::OnDamage(const float damage)
{
	math::Decrease(m_current_health, damage, 0.0f);
}

void Health::SetCurrentMaxHealth(const float current_max_health)
{
	m_current_max_health = current_max_health;
	if (m_current_max_health > m_max_health)
	{
		m_current_max_health = m_max_health;
	}
}
