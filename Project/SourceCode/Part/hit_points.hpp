#pragma once
#include "../Calculation/math.hpp"
#include "../Data/Kind/hit_points_part_kind.hpp"

class HitPoints final
{
public:
	HitPoints(const float max_hit_points);
	~HitPoints();

	/// @brief ‰ñ•¡‚·‚é
	void Recover(const float recover_points);

	/// @brief ƒ_ƒ[ƒW‚ðŽó‚¯‚½
	void OnDamage(const float damage);

	void SetMaxHitPoints(const float max_hit_points) { m_max_hit_points = max_hit_points; }

	[[nodiscard]] float	GetCurrentHitPoints() const { return m_current_hit_points; }
	[[nodiscard]] bool	IsAlive()			  const { return m_current_hit_points > 0.0f; }

private:
	float				m_max_hit_points;
	float				m_current_hit_points;
};
