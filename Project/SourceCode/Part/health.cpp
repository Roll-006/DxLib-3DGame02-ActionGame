#include "health.hpp"

Gauge::Gauge(const float max_health, const float max_current_health) :
	m_current_value	(max_current_health),
	m_prev_value		(max_current_health),
	m_current_max_value(max_current_health),
	m_max_value		(max_health)
{

}

Gauge::~Gauge()
{

}

void Gauge::IncreaseMax()
{
	m_current_value = m_current_max_value;
}

void Gauge::Increase(const float increase_value)
{
	math::Increase(m_current_value, increase_value, m_current_max_value, false);
}

void Gauge::Decrease(const float decrease_value)
{
	m_prev_value = m_current_value;
	math::Decrease(m_current_value, decrease_value, 0.0f);
}

void Gauge::SetCurrentMaxValue(const float current_max_health)
{
	m_current_max_value = current_max_health;
	if (m_current_max_value > m_max_value)
	{
		m_current_max_value = m_max_value;
	}
}
