#pragma once

class IEnemy abstract
{
public:
	virtual ~IEnemy() = default;

protected:
	#pragma region コライダー
	/// @brief 脚トリガーを作成
	virtual void CreateLegTrigger(const float up_leg_capsule_radius, const float down_leg_capsule_radius) abstract;
	/// @brief 頭部トリガーを作成
	virtual void CreateHeadTrigger(const float sphere_radius) abstract;
	/// @brief 胴体トリガーを作成
	virtual void CreateBodyTrigger(const float capsule_radius) abstract;

	/// @brief カプセルの位置を計算
	virtual void CalcLegTriggerPos()  abstract;
	virtual void CalcBodyTriggerPos() abstract;
	#pragma endregion
};
