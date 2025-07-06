#pragma once
#include "chara_base.hpp"

class EnemyBase abstract : public CharaBase
{
public:
	EnemyBase(const std::string& name, const std::string& file_path, const MassKind mass_level_kind);
	virtual ~EnemyBase() = default;

protected:
	#pragma region コライダー
	/// @brief 脚トリガーを作成
	void CreateLegTrigger (const float up_leg_capsule_radius, const float down_leg_capsule_radius);
	/// @brief 頭部トリガーを作成
	void CreateHeadTrigger(const float sphere_radius);
	/// @brief 胴体トリガーを作成
	void CreateBodyTrigger(const float capsule_radius);

	/// @brief カプセルの位置を計算
	void CalcLegTriggerPos();
	void CalcBodyTriggerPos();
	#pragma endregion

protected:
	VECTOR m_move_dir;
	VECTOR m_look_dir;
};
