#pragma once
#include "character_base.hpp"

class EnemyBase abstract : public CharacterBase
{
public:
	EnemyBase(const std::string& name, const MassKind mass_level_kind);
	virtual ~EnemyBase() = default;

	virtual void AttachTarget(const std::shared_ptr<CharacterBase> target_character) abstract;
	virtual void DetachTarget() abstract;

	void SetAttackIntervalTime();
	void CalcAttackIntervalTime();

	/// @brief 行動を起こすを許可される
	void OnAllowAction()		{ m_can_action = true; }
	/// @brief 行動が強制的に停止させられる
	void OnStopActionForcibly() { m_can_action = false; }

	[[nodiscard]] bool IsTargetInSight() const { return m_is_target_in_sight; }
	[[nodiscard]] bool CanAttack()		 const { return m_attack_interval_timer <= 0.0f; }
	[[nodiscard]] bool CanAction()		 const { return m_can_action; }

protected:
	float m_attack_interval_time;
	float m_attack_interval_timer;
	bool  m_can_action;
	bool  m_is_target_in_sight;
};
