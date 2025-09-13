#pragma once
#include "../Calculation/math.hpp"
#include "../Kind/health_part_kind.hpp"

class Health final
{
public:
	Health(const float max_health, const float max_current_health);
	~Health();

	/// @brief 回複する
	void Recover(const float recover_points);

	/// @brief ダメージを受けた
	void OnDamage(const float damage);

	/// @brief 現在の上限値を設定する
	void SetCurrentMaxHealth(const float current_max_health);

	[[nodiscard]] float	GetCurrentHealth()		const { return m_current_health; }
	[[nodiscard]] float	GetCurrentMaxHealth()	const { return m_current_max_health; }
	[[nodiscard]] float	GetMaxHealth()			const { return m_max_health; }
	[[nodiscard]] bool	IsAlive()				const { return m_current_health > 0.0f; }

private:
	float m_current_health;
	float m_current_max_health;
	float m_max_health;
};
