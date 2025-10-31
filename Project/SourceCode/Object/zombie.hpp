#pragma once
#include "../Base/enemy_base.hpp"
#include "../Interface/i_melee_hittable.hpp"
#include "../Interface/i_grabber.hpp"
#include "../Interface/i_stealth_killable.hpp"

#include "../Animator/zombie_animator.hpp"
#include "../Data/zombie_collider_data.hpp"

class ZombieStateController;

class Zombie final : public EnemyBase, public IMeleeHittable, public IGrabber, public IStealthKillable
{
public:
	Zombie(const std::string& enemy_id);
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

	[[nodiscard]] float GetDamageOverTimeStartTime()	const override	{ return damage_over_time_start_time; }
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
	std::string model_path;

	VECTOR basic_angle;
	float  basic_scale;

	float  walk_speed;
	float  run_speed;
	float  run_grab_speed;
	float  move_dir_offset_speed;
	float  look_dir_offset_speed;

	ZombieColliderData collider_data;

	float  damage_over_time_start_time;

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

	data.at("model_path")					.get_to(zombie.model_path);
	data.at("basic_angle")					.get_to(zombie.basic_angle);
	data.at("basic_scale")					.get_to(zombie.basic_scale);

	data.at("walk_speed")					.get_to(zombie.walk_speed);
	data.at("run_speed")					.get_to(zombie.run_speed);
	data.at("run_grab_speed")				.get_to(zombie.run_grab_speed);
	data.at("move_dir_offset_speed")		.get_to(zombie.m_move_dir_offset_speed);
	data.at("look_dir_offset_speed")		.get_to(zombie.m_look_dir_offset_speed);

	data.at("collider_data")				.get_to(zombie.collider_data);

	data.at("damage_over_time_start_time")	.get_to(zombie.damage_over_time_start_time);
}

inline void to_json(nlohmann::json& data, const Zombie& zombie)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const EnemyBase&>(zombie));

	nlohmann::json derived_json =
	{
		{ "model_path",						zombie.model_path },
		{ "basic_angle",					zombie.basic_angle },
		{ "basic_scale",					zombie.basic_scale },

		{ "walk_speed",						zombie.walk_speed },
		{ "run_speed",						zombie.run_speed },
		{ "run_grab_speed",					zombie.run_grab_speed },
		{ "move_dir_offset_speed",			zombie.m_move_dir_offset_speed },
		{ "look_dir_offset_speed",			zombie.m_look_dir_offset_speed },

		{ "collider_data",					zombie.collider_data },

		{ "damage_over_time_start_time",	zombie.damage_over_time_start_time }
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
