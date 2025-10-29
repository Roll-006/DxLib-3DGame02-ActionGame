#pragma once
#include "character_base.hpp"
#include "../Part/patrol_route_giver.hpp"

class EnemyBase abstract : public CharacterBase
{
public:
	EnemyBase(const std::string& name, const MassKind mass_level_kind);
	virtual ~EnemyBase() = default;

	virtual void AttachTarget(const std::shared_ptr<CharacterBase>& target_character) abstract;
	virtual void DetachTarget() abstract;

	void SetAttackIntervalTime();
	void CalcAttackIntervalTime();

	void CreatePatrolPos(const PatrolRouteGiver::PatrolKind patrol_kind, const std::string& route_id);
	void ChangePatrolDestination();

	/// @brief リスポーンさせられる
	virtual void OnRespawn(const VECTOR& pos, const VECTOR& look_dir) abstract;

	/// @brief 行動を起こすを許可される
	/// @brief また、同時に攻撃インターバル時間が1/4未満の場合、
	/// @brief 1/2の時間が付与される
	void OnAllowAction();
	/// @brief 行動が強制的に停止させられる
	void OnDisallowActionForcibly() { m_is_disallow_action_forcibly = true; }

	[[nodiscard]] int								GetEnemyHandle()		const { return m_enemy_handle; }
	[[nodiscard]] std::shared_ptr<PatrolRouteGiver> GetPatrolRouteGiver()	const { return m_patrol_route_giver; }
	[[nodiscard]] bool								IsTargetInSight()		const { return !m_is_lost_target; }
	[[nodiscard]] bool								CanAttack()				const { return m_attack_interval_timer <= 0.0f; }
	[[nodiscard]] bool								CanAction()				const { return m_can_action; }

protected:
	virtual void JudgeAction() abstract;
	void JudgeLostTarget();

protected:
	std::shared_ptr<PatrolRouteGiver> m_patrol_route_giver;
	VECTOR m_patrol_destination_pos;

	float m_attack_interval_time;
	float m_attack_interval_timer;
	bool  m_can_action;
	bool  m_is_disallow_action_forcibly;

	bool  m_on_collided_vision_trigger;
	bool  m_has_obstacle_between_target;
	bool  m_is_lost_target;

private:
	int	  m_enemy_handle;
};
