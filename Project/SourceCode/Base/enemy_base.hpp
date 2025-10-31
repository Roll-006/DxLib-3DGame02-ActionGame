#pragma once
#include "character_base.hpp"
#include "../Part/patrol_route_giver.hpp"

class EnemyBase abstract : public CharacterBase
{
public:
	EnemyBase(const std::string& name);
	virtual ~EnemyBase() = default;

	virtual void AttachTarget(const std::shared_ptr<CharacterBase>& target_character) abstract;
	virtual void DetachTarget() abstract;

	void SetAttackIntervalTime();
	void CalcAttackIntervalTime();

	void CreatePatrolPos(const PatrolRouteGiver::PatrolKind patrol_kind, const std::string& route_id);
	void ChangePatrolDestination();

	/// @brief リスポーンさせられる
	virtual void OnRespawn(const VECTOR& pos, const VECTOR& look_dir) abstract;

	void OnDetected();
	void Disappear();

	/// @brief 行動を起こすを許可される
	/// @brief また、同時に攻撃インターバル時間が1/4未満の場合、
	/// @brief 1/2の時間が付与される
	void OnAllowAction();
	/// @brief 行動が強制的に停止させられる
	void OnDisallowActionForcibly() { m_is_disallow_action_forcibly = true; }

	[[nodiscard]] int								GetEnemyHandle()		const { return enemy_handle; }
	[[nodiscard]] std::shared_ptr<PatrolRouteGiver> GetPatrolRouteGiver()	const { return m_patrol_route_giver; }
	[[nodiscard]] bool								IsTargetInSight()		const { return m_is_target_in_sight; }
	[[nodiscard]] bool								CanAttack()				const { return m_attack_interval_timer <= 0.0f; }
	[[nodiscard]] bool								CanAction()				const { return m_can_action; }

protected:
	virtual void JudgeAction() abstract;
	void JudgeTargetInSight();

protected:
	int									enemy_handle;
	bool								use_patrol;
	std::string							route_id;
	PatrolRouteGiver::PatrolKind		patrol_kind;
	float								attack_interval_time;

	std::shared_ptr<PatrolRouteGiver>	m_patrol_route_giver;
	VECTOR								m_patrol_destination_pos;

	float m_attack_interval_timer;
	bool  m_can_action;
	bool  m_is_disallow_action_forcibly;

	bool  m_on_collided_vision_trigger;
	bool  m_has_obstacle_between_target;
	bool  m_is_lost_target;
	bool  m_is_target_in_sight;

private:
	friend void from_json(const nlohmann::json& data, EnemyBase& enemy_base);
	friend void to_json  (nlohmann::json& data, const EnemyBase& enemy_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, EnemyBase& enemy_base)
{
	from_json(data, static_cast<CharacterBase&>(enemy_base));

	data.at("use_patrol")			.get_to(enemy_base.use_patrol);
	data.at("route_id")				.get_to(enemy_base.route_id);
	data.at("patrol_kind")			.get_to(enemy_base.patrol_kind);
	data.at("attack_interval_time")	.get_to(enemy_base.attack_interval_time);
}

inline void to_json(nlohmann::json& data, const EnemyBase& enemy_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const CharacterBase&>(enemy_base));

	nlohmann::json derived_json =
	{
		{ "use_patrol",				enemy_base.use_patrol },
		{ "route_id",				enemy_base.route_id },
		{ "patrol_kind",			enemy_base.patrol_kind },
		{ "attack_interval_time",	enemy_base.attack_interval_time },
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
