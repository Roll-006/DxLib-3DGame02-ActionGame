#pragma once
#include "../Calculation/math.hpp"
#include "../Kind/hit_points_part_kind.hpp"

class HitPoints final
{
public:
	HitPoints(const float max_hit_points, const float max_current_hit_points);
	~HitPoints();

	/// @brief 回複する
	void Recover(const float recover_points);

	/// @brief ダメージを受けた
	void OnDamage(const float damage);

	/// @brief 現在の上限値を設定する
	void SetCurrentMaxHitPoints(const float current_max_hit_points);

	[[nodiscard]] float	GetCurrentHitPoints()		const { return m_current_hit_points; }
	[[nodiscard]] float	GetCurrentMaxHitPoints()	const { return m_current_max_hit_points; }
	[[nodiscard]] float	GetMaxHitPoints()			const { return m_max_hit_points; }
	[[nodiscard]] bool	IsAlive()					const { return m_current_hit_points > 0.0f; }

private:
	float m_current_hit_points;
	float m_current_max_hit_points;
	float m_max_hit_points;
};
