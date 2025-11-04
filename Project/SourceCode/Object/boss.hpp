#pragma once
#include "../Base/enemy_base.hpp"
#include "../Interface/i_poolable.hpp"
#include "../Interface/i_melee_hittable.hpp"
#include "../Interface/i_grabber.hpp"
#include "../Interface/i_stealth_killable.hpp"

#include "../Animator/boss_animator.hpp"
#include "../Data/humanoid_enemy_collider_data.hpp"

class BossStateController;

class Boss final : public EnemyBase, public IPoolable, public IMeleeHittable, public IStealthKillable
{
public:
	Boss(const std::string& id);
	~Boss() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void AllowReturnPool() { m_is_return_pool = true; }

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnDamage(const HealthPartKind part_kind, const float damage) override;

	void AttachTarget(const std::shared_ptr<CharacterBase>& target_character) override;
	void DetachTarget() override;

	void OnRespawn(const VECTOR& pos, const VECTOR& look_dir) override;
	void Detected();
	

	#pragma region メレー
	[[nodiscard]] bool  IsStandStun()  const override;
	[[nodiscard]] bool  IsCrouchStun() const override;
	#pragma endregion


	#pragma region ステルスキル
	void OnStealthKill()		override;
	void ExitStealthKilled()	override;
	void DisallowStealthKill()	override { m_is_allow_stealth_kill = false; }

	[[nodiscard]] bool IsAllowStealthKill()		const override	{ return m_is_allow_stealth_kill; }
	[[nodiscard]] bool IsStealthKillerInSight()	const override	{ return IsDetectedTarget(); }
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
	[[nodiscard]] float									GetDeltaTime()		 const	override;
	[[nodiscard]] std::shared_ptr<BossStateController>	GetStateController() const				{ return m_state; }
	[[nodiscard]] bool									IsReturnPool()				override	{ return m_is_return_pool; }
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

	HumanoidEnemyColliderData collider_data;

	float  damage_over_time_start_time;

private:
	std::shared_ptr<BossStateController>	m_state;

	bool m_is_return_pool;
	bool m_is_allow_stealth_kill;
	bool m_on_stealth_kill;

	friend void from_json(const nlohmann::json& data, Boss& boss);
	friend void to_json  (nlohmann::json& data, const Boss& boss);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Boss& boss)
{
	from_json(data, static_cast<EnemyBase&>(boss));

	data.at("model_path")					.get_to(boss.model_path);
	data.at("basic_angle")					.get_to(boss.basic_angle);
	data.at("basic_scale")					.get_to(boss.basic_scale);

	data.at("walk_speed")					.get_to(boss.walk_speed);
	data.at("run_speed")					.get_to(boss.run_speed);
	data.at("run_grab_speed")				.get_to(boss.run_grab_speed);
	data.at("move_dir_offset_speed")		.get_to(boss.m_move_dir_offset_speed);
	data.at("look_dir_offset_speed")		.get_to(boss.m_look_dir_offset_speed);

	data.at("collider_data")				.get_to(boss.collider_data);

	data.at("damage_over_time_start_time")	.get_to(boss.damage_over_time_start_time);
}

inline void to_json(nlohmann::json& data, const Boss& boss)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const EnemyBase&>(boss));

	nlohmann::json derived_json =
	{
		{ "model_path",						boss.model_path },
		{ "basic_angle",					boss.basic_angle },
		{ "basic_scale",					boss.basic_scale },

		{ "walk_speed",						boss.walk_speed },
		{ "run_speed",						boss.run_speed },
		{ "run_grab_speed",					boss.run_grab_speed },
		{ "move_dir_offset_speed",			boss.m_move_dir_offset_speed },
		{ "look_dir_offset_speed",			boss.m_look_dir_offset_speed },

		{ "collider_data",					boss.collider_data },

		{ "damage_over_time_start_time",	boss.damage_over_time_start_time }
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
