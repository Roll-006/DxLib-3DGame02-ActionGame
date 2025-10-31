#pragma once
#include "../Base/enemy_base.hpp"
#include "../Interface/i_melee_hittable.hpp"
#include "../Interface/i_grabber.hpp"
#include "../Interface/i_stealth_killable.hpp"

#include "../Animator/zombie_animator.hpp"

class ZombieStateController;

class Zombie final : public EnemyBase, public IMeleeHittable, public IGrabber, public IStealthKillable
{
public:
	Zombie();
	~Zombie() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnDamage(const HealthPartKind part_kind, const float damage) override;

	void AttachTarget(const std::shared_ptr<CharacterBase>& target_character) override;
	void DetachTarget() override;

	void OnRespawn(const VECTOR& pos, const VECTOR& look_dir) override;
	

	#pragma region 掴み
	void Grab()			override;
	void Release()		override;
	void OnEscape()		override;

	[[nodiscard]] float GetDamageOverTimeStartTime()	const override	{ return m_damage_over_time_start_time; }
	[[nodiscard]] bool	IsTargetEscaped()				const override  { return m_is_target_escaped; }
	#pragma endregion


	#pragma region メレー
	[[nodiscard]] bool  IsStandStun()  const override;
	[[nodiscard]] bool  IsCrouchStun() const override;
	#pragma endregion


	#pragma region ステルスキル
	void OnStealthKill()		override;
	void ExitStealthKilled()	override;
	void DisallowStealthKill()	override { m_is_allow_stealth_kill = false; }

	[[nodiscard]] bool IsAllowStealthKill()		const override	{ return m_is_allow_stealth_kill; }
	[[nodiscard]] bool IsStealthKillerInSight()	const override	{ return IsTargetInSight(); }
	[[nodiscard]] bool IsStealthKilled()		const override	{ return m_on_stealth_kill; }
	#pragma endregion


	#pragma region State
	void Move();
	void TrackMove(const VECTOR& target_pos);

	void UpdateGrabRun();

	void CalcMoveSpeed();
	void CalcMoveSpeedStop();
	void CalcMoveSpeedRun();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] float										GetDeltaTime()			const override;
	[[nodiscard]] std::shared_ptr<ZombieStateController>	GetStateController()	const	{ return m_state; }
	[[nodiscard]] bool										CanGrabTarget()			const	{ return m_can_grab_target; }
	#pragma endregion

private:
	void JudgeAction() override;

private:
	std::string m_model_path;

	VECTOR m_basic_angle;
	float  m_basic_scale;

	float  m_walk_speed;
	float  m_run_speed;
	float  m_run_grab_speed;
	float  m_move_dir_offset_speed;
	float  m_look_dir_offset_speed;

	float  m_fov;
	float  m_visible_distance;
	float  m_capsule_radius;
	float  m_collision_area_radius;
	VECTOR m_collision_area_offset;
	float  m_landing_trigger_radius;
	float  m_head_trigger_radius;
	float  m_up_body_trigger_radius;
	float  m_down_body_trigger_radius;
	float  m_upper_arm_trigger_radius;
	float  m_forearm_trigger_radius;
	float  m_hand_trigger_radius;
	float  m_up_leg_trigger_radius;
	float  m_down_leg_trigger_radius;

	float  m_damage_over_time_start_time;

	//static constexpr VECTOR kBasicAngle				= { 0.0f, DX_PI_F, 0.0f };
	//static constexpr float  kBasicScale				= 0.3f;
	//
	//static constexpr float kFOV						= 80.0f;
	//static constexpr float kVisibleDistance			= 300.0f;
	//
	//static constexpr float kAttackIntervalTime		= 3.0f;
	//static constexpr float kInvincibleTime			= 0.5f;
	//
	//static constexpr float kWalkSpeed				= 60.0f;
	//static constexpr float kRunSpeed				= 100.0f;
	//static constexpr float kRunGrabSpeed			= 110.0f;
	//
	//static constexpr float kMoveDirOffsetSpeed		= 5.0f;			// 移動方向の補正速度
	//static constexpr float kLookDirOffsetSpeed		= 3.0f;			// 見る方向の補正角度
	//
	//static constexpr float  kCapsuleRadius			= 8.0f;
	//static constexpr float  kCollisionAreaRadius	= 40.0f;
	//static constexpr VECTOR kCollisionAreaOffset	= { 0.0f, 30.0f, 0.0f };
	//static constexpr float  kLandingTriggerRadius	= 6.0f;
	//static constexpr float  kHeadTriggerRadius		= 5.0f;
	//static constexpr float  kUpBodyTriggerRadius	= 6.0f;
	//static constexpr float  kDownBodyTriggerRadius	= 6.0f;
	//static constexpr float  kUpperArmTriggerRadius	= 2.5f;
	//static constexpr float  kForearmTriggerRadius	= 2.2f;
	//static constexpr float  kHandTriggerRadius		= 2.0f;
	//static constexpr float  kUpLegTriggerRadius		= 3.5f;
	//static constexpr float  kDownLegTriggerRadius	= 2.5f;
	//
	//static constexpr float kDamageOverTimeStartTime = 1.0f;

private:
	std::shared_ptr<ZombieStateController>	m_state;

	bool m_can_grab_target;
	bool m_is_target_escaped;
	bool m_is_allow_stealth_kill;
	bool m_on_stealth_kill;

	friend void from_json(const nlohmann::json& data, Zombie& zombie);
	friend void to_json  (nlohmann::json& data, const Zombie& zombie);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Zombie& zombie)
{
	from_json(data, static_cast<EnemyBase&>(zombie));

	data.at("model_path")					.get_to(zombie.m_model_path);
	data.at("basic_angle")					.get_to(zombie.m_basic_angle);
	data.at("basic_scale")					.get_to(zombie.m_basic_scale);

	data.at("walk_speed")					.get_to(zombie.m_walk_speed);
	data.at("run_speed")					.get_to(zombie.m_run_speed);
	data.at("run_grab_speed")				.get_to(zombie.m_run_grab_speed);
	data.at("move_dir_offset_speed")		.get_to(zombie.m_move_dir_offset_speed);
	data.at("look_dir_offset_speed")		.get_to(zombie.m_look_dir_offset_speed);

	data.at("fov")							.get_to(zombie.m_fov);
	data.at("visible_distance")				.get_to(zombie.m_visible_distance);
	data.at("capsule_radius")				.get_to(zombie.m_capsule_radius);
	data.at("collision_area_radius")		.get_to(zombie.m_collision_area_radius);
	data.at("collision_area_offset")		.get_to(zombie.m_collision_area_offset);
	data.at("landing_trigger_radius")		.get_to(zombie.m_landing_trigger_radius);
	data.at("head_trigger_radius")			.get_to(zombie.m_head_trigger_radius);
	data.at("up_body_trigger_radius")		.get_to(zombie.m_up_body_trigger_radius);
	data.at("down_body_trigger_radius")		.get_to(zombie.m_down_body_trigger_radius);
	data.at("upper_arm_trigger_radius")		.get_to(zombie.m_upper_arm_trigger_radius);
	data.at("forearm_trigger_radius")		.get_to(zombie.m_forearm_trigger_radius);
	data.at("hand_trigger_radius")			.get_to(zombie.m_hand_trigger_radius);
	data.at("up_leg_trigger_radius")		.get_to(zombie.m_up_leg_trigger_radius);
	data.at("down_leg_trigger_radius")		.get_to(zombie.m_down_leg_trigger_radius);

	data.at("damage_over_time_start_time")	.get_to(zombie.m_damage_over_time_start_time);
}

inline void to_json(nlohmann::json& data, const Zombie& zombie)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const EnemyBase&>(zombie));

	nlohmann::json derived_json =
	{
		{ "model_path",						zombie.m_model_path },
		{ "basic_angle",					zombie.m_basic_angle },
		{ "basic_scale",					zombie.m_basic_scale },

		{ "walk_speed",						zombie.m_walk_speed },
		{ "run_speed",						zombie.m_run_speed },
		{ "run_grab_speed",					zombie.m_run_grab_speed },
		{ "move_dir_offset_speed",			zombie.m_move_dir_offset_speed },
		{ "look_dir_offset_speed",			zombie.m_look_dir_offset_speed },

		{ "fov",							zombie.m_fov },
		{ "visible_distance",				zombie.m_visible_distance },
		{ "capsule_radius",					zombie.m_capsule_radius },
		{ "collision_area_radius",			zombie.m_collision_area_radius },
		{ "collision_area_offset",			zombie.m_collision_area_offset },
		{ "landing_trigger_radius",			zombie.m_landing_trigger_radius },
		{ "head_trigger_radius",			zombie.m_head_trigger_radius },
		{ "up_body_trigger_radius",			zombie.m_up_body_trigger_radius },
		{ "down_body_trigger_radius",		zombie.m_down_body_trigger_radius },
		{ "upper_arm_trigger_radius",		zombie.m_upper_arm_trigger_radius },
		{ "forearm_trigger_radius",			zombie.m_forearm_trigger_radius },
		{ "hand_trigger_radius",			zombie.m_hand_trigger_radius },
		{ "up_leg_trigger_radius",			zombie.m_up_leg_trigger_radius },
		{ "down_leg_trigger_radius",		zombie.m_down_leg_trigger_radius },

		{ "damage_over_time_start_time",	zombie.m_damage_over_time_start_time }
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
