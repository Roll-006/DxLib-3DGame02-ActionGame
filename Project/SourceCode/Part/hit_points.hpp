#pragma once
#include "../Calculation/math.hpp"

class HitPoints final
{
public:
	enum class BodyPartKind
	{
		kHead,		// 頭部
		kBody,		// 胴体
		kLeftArm,	// 左腕
		kRightArm,	// 右腕
		kLeftLeg,	// 左足
		kRightLeg,	// 右足
	};

public:
	HitPoints();
	~HitPoints();

	/// @brief 回複する
	void Recover(const float recover_points);

	/// @brief ダメージを受けた
	/// @param damage ダメージ量
	/// @param on_damage_part ダメージを受けた部位
	void OnDamage(const float damage, const BodyPartKind on_damage_part);

	[[nodiscard]] float GetCurrentHitPoints(const BodyPartKind body_part_kind) const;
	[[nodiscard]] bool  IsAlive() const { return m_current_all_hit_points <= 0; }

private:
	float m_max_hit_points;
	float m_current_all_hit_points;
	std::unordered_map<BodyPartKind, float> m_current_hit_points;
};
