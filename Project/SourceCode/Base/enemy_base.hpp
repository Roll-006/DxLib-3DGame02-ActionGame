#pragma once
#include "character_base.hpp"
#include "../Part/patrol_route_giver.hpp"
#include "../Kind/rot_dir_kind.hpp"

class EnemyBase abstract : public CharacterBase
{
public:
	EnemyBase(const std::string& name);
	virtual ~EnemyBase() = default;

	virtual void AttachTarget(const std::shared_ptr<CharacterBase>& target_character) abstract;
	virtual void DetachTarget() abstract;

	void SetUpAttackIntervalTime();
	void CalcAttackIntervalTime();

	void CreatePatrolPos(const PatrolRouteGiver::PatrolKind patrol_kind, const std::string& route_id);
	void ChangePatrolDestination();

	/// @brief リスポーンさせられる
	virtual void OnRespawn(const VECTOR& pos, const VECTOR& look_dir) abstract;

	/// @brief Forwardが回転させられる
	/// @param angle 回転する角度 (ラジアン)
	/// @param rot_dir_kind 回転方向
	void OnRotate(const float angle, const RotDirKind rot_dir_kind);

	void OnDetected();
	void Disappear();

	/// @brief 行動を起こすを許可される
	/// @brief また、同時に攻撃インターバル時間が1/4未満の場合、
	/// @brief 1/2の時間が付与される
	void OnAllowAction();
	/// @brief 行動が強制的に停止させられる
	void OnDisallowActionForcibly() { m_is_disallow_action_forcibly = true; }

	void DisallowDecreaseKnockBackGauge() { m_can_decrease_knock_back_gauge = false; }


	#pragma region Getter
	[[nodiscard]] std::string						GetEnemyID()				const { return enemy_id; }
	[[nodiscard]] std::shared_ptr<PatrolRouteGiver> GetPatrolRouteGiver()		const { return m_patrol_route_giver; }
	[[nodiscard]] std::shared_ptr<Gauge>			GetKnockBackGauge()			const { return m_knock_back_gauge; }
	[[nodiscard]] bool								IsDetectedTarget()			const { return m_is_detected_target; }
	[[nodiscard]] bool								IsPrevDetectedTarget()		const { return m_is_prev_detected_target; }
	[[nodiscard]] bool								CanAttack()					const { return m_attack_interval_timer <= 0.0f; }
	[[nodiscard]] bool								CanAction()					const { return m_can_action; }
	[[nodiscard]] float								GetDetecteNotifyDistance()	const { return detected_notify_distance; }
	#pragma endregion

protected:
	virtual void JudgeAction() abstract;
	void JudgeTargetInSight();

protected:
	std::string							enemy_id;
	bool								use_patrol;
	std::string							route_id;
	PatrolRouteGiver::PatrolKind		patrol_kind;
	float								attack_interval_time;
	float								detected_notify_distance;

	std::shared_ptr<PatrolRouteGiver>	m_patrol_route_giver;
	VECTOR								m_patrol_destination_pos;

	float m_attack_interval_timer;
	bool  m_can_action;
	bool  m_is_disallow_action_forcibly;
	float m_disappear_down_speed;

	bool  m_on_collided_vision_trigger;
	bool  m_has_obstacle_between_target;
	bool  m_is_detected_target;				// 発見状態
	bool  m_is_prev_detected_target;		// 1フレーム前の発見状態
	bool  m_is_detection_shared;			// 発見状態が共有された

	std::shared_ptr<Gauge> m_knock_back_gauge;
	bool  m_can_decrease_knock_back_gauge;

private:
	friend void from_json(const nlohmann::json& j_data, EnemyBase& enemy_base);
	friend void to_json  (nlohmann::json& j_data, const EnemyBase& enemy_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, EnemyBase& enemy_base)
{
	from_json(j_data, static_cast<CharacterBase&>(enemy_base));

	j_data.at("use_patrol")			.get_to(enemy_base.use_patrol);
	j_data.at("route_id")				.get_to(enemy_base.route_id);
	j_data.at("patrol_kind")			.get_to(enemy_base.patrol_kind);
	j_data.at("attack_interval_time")	.get_to(enemy_base.attack_interval_time);
}

inline void to_json(nlohmann::json& j_data, const EnemyBase& enemy_base)
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

	j_data = base_json;
	j_data.update(derived_json);
}
#pragma endregion
