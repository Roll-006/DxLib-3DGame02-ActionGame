#pragma once
#include "../Calculation/math.hpp"

class Gauge final
{
public:
	Gauge(const float max_health, const float max_current_health);
	~Gauge();

	/// @brief 回複する
	void Increase(const float increase_value);

	/// @brief 最大値まで回復する
	void IncreaseMax();

	/// @brief ダメージを受けた
	void Decrease(const float decrease_value);

	/// @brief 現在の上限値を設定する
	void SetCurrentMaxValue(const float current_max_value);


	#pragma region Getter
	[[nodiscard]] float	GetCurrentValue()		const { return m_current_value; }

	/// @brief 以前のHPを取得する
	/// @brief WARNING : 1フレーム前のHPではないことに注意
	[[nodiscard]] float	GetPrevValue()			const { return m_prev_value; }

	[[nodiscard]] float	GetCurrentMaxValue()	const { return m_current_max_value; }
	[[nodiscard]] float	GetMaxValue()			const { return m_max_value; }
	[[nodiscard]] bool	IsAlive()				const { return m_current_value > 0.0f; }
	#pragma endregion

private:
	float m_current_value;
	float m_prev_value;
	float m_current_max_value;
	float m_max_value;

	friend void from_json	(const nlohmann::json& data, Gauge& gauge);
	friend void to_json		(nlohmann::json& data, const Gauge& gauge);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Gauge& gauge)
{
	data.at("current_value")		.get_to(gauge.m_current_value);
	data.at("prev_value")			.get_to(gauge.m_prev_value);
	data.at("current_max_value")	.get_to(gauge.m_current_max_value);
	data.at("max_value")			.get_to(gauge.m_max_value);
}

inline void to_json(nlohmann::json& data, const Gauge& gauge)
{
	data = nlohmann::json
	{
		{ "current_value",		gauge.m_current_value },
		{ "prev_value",			gauge.m_prev_value },
		{ "current_max_value",	gauge.m_current_max_value },
		{ "max_value",			gauge.m_max_value }
	};
}
#pragma endregion
