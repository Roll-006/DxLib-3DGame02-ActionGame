#pragma once
#include "../Calculation/math.hpp"
#include "../Kind/health_part_kind.hpp"
#include "subject.hpp"

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
	[[nodiscard]] std::shared_ptr<Subject<Health>> GetSubject() const { return m_subject; }
	[[nodiscard]] bool	IsAlive()				const { return m_current_health > 0.0f; }

private:
	float m_current_health;
	float m_current_max_health;
	float m_max_health;
	std::shared_ptr<Subject<Health>> m_subject;

	friend void from_json	(const nlohmann::json& data, Health& health);
	friend void to_json		(nlohmann::json& data, const Health& health);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Health& health)
{
	data.at("current_health")		.get_to(health.m_current_health);
	data.at("current_max_health")	.get_to(health.m_current_max_health);
	data.at("max_health")			.get_to(health.m_max_health);
}

inline void to_json(nlohmann::json& data, const Health& health)
{
	data = nlohmann::json
	{
		{ "current_health",		health.m_current_health },
		{ "current_max_health",	health.m_current_max_health },
		{ "max_health",			health.m_max_health }
	};
}
#pragma endregion
