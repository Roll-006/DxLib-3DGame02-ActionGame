#pragma once
#include "../Base/enemy_base.hpp"
#include "../Interface/i_poolable.hpp"
#include "../Interface/i_humanoid.hpp"
#include "../Interface/i_melee_hittable.hpp"
#include "../Interface/i_grabber.hpp"
#include "../Interface/i_stealth_killable.hpp"

#include "../InverseKinematics/humanoid_arm_ik_solver.hpp"
#include "../InverseKinematics/humanoid_foot_ik_solver.hpp"

#include "../Data/zombie_data.hpp"
#include "../State/ZombieState/zombie_state.hpp"
#include "../AI/zombie_ai.hpp"

#include "../Data/humanoid_arm_ray_data.hpp"
#include "../Data/humanoid_leg_ray_data.hpp"

class Zombie final : public EnemyBase, public IPoolable, public IHumanoid, public IMeleeHittable, public IGrabber, public IStealthKillable
{
public:
	Zombie(const std::string& id);
	~Zombie() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void AllowReturnPool() { m_is_return_pool = true; }

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;
	void OnDamage(const HealthPartKind part_kind, const float damage) override;

	void AttachTarget(const std::shared_ptr<CharacterBase>& target_character) override;
	void DetachTarget() override;

	void OnRespawn(const VECTOR& pos, const VECTOR& look_dir) override;
	void Detected();


	#pragma region Humanoid
	[[nodiscard]] const std::shared_ptr<const HumanoidFrameGetter> GetHumanoidFrame() const override { return m_humanoid_frame; }
	#pragma endregion
	

	#pragma region 掴み
	void Grab()			override;
	void Release()		override;
	void OnEscape()		override;

	[[nodiscard]] const float	GetDamageOverTimeStartTime()	const override	{ return data.damage_over_time_start_time; }
	[[nodiscard]] const bool	IsTargetEscaped()				const override  { return m_is_target_escaped; }
	#pragma endregion


	#pragma region メレー
	[[nodiscard]] const bool IsStandStun()  const override;
	[[nodiscard]] const bool IsCrouchStun() const override;
	#pragma endregion


	#pragma region ステルスキル
	void OnStealthKill()		override;
	void ExitStealthKilled()	override;
	void DisallowStealthKill()	override { m_is_allow_stealth_kill = false; }

	[[nodiscard]] const bool IsAllowStealthKill()		const override	{ return m_is_allow_stealth_kill; }
	[[nodiscard]] const bool IsStealthKillerInSight()	const override	{ return IsDetectedTarget(); }
	[[nodiscard]] const bool IsStealthKilled()			const override	{ return m_on_stealth_kill; }
	#pragma endregion


	#pragma region State
	void TrackMove(const VECTOR& target_pos, const bool is_distance_limit);

	void OnFootIK();
	void OnLeftCrouchIK();
	void OnRightCrouchIK();

	void UpdateGrabRun();

	void CalcMoveSpeed();
	void CalcMoveSpeedStop();
	void CalcMoveSpeedRun();
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] const float										GetDeltaTime()				const	override;
	[[nodiscard]] const std::shared_ptr<CharacterBase>				GetTarget()					const	{ return m_ai->GetTarget(); }
	[[nodiscard]] const bool										CanGrabTarget()				const	{ return m_can_grab_target; }
	[[nodiscard]] const bool										IsReturnPool() override				{ return m_is_return_pool; }
	[[nodiscard]] const std::shared_ptr<const HumanoidArmIKSolver>	GetHumanoidArmIKSolver()	const	{ return m_humanoid_arm_ik; }
	[[nodiscard]] const std::shared_ptr<const HumanoidFootIKSolver>	GetHumanoidFootIKSolver()	const	{ return m_humanoid_foot_ik; }
	#pragma endregion

private:
	void JudgeAction() override;

private:
	ZombieData data;

	HumanoidArmRayData						m_arm_ray_data;
	HumanoidLegRayData						m_leg_ray_data;
	std::shared_ptr<HumanoidArmIKSolver>	m_humanoid_arm_ik;
	std::shared_ptr<HumanoidFootIKSolver>	m_humanoid_foot_ik;
	std::shared_ptr<HumanoidFrameGetter>	m_humanoid_frame;

	std::shared_ptr<zombie_state::State>	m_state;
	std::shared_ptr<ZombieAI>				m_ai;
	bool									m_is_return_pool;
	bool									m_can_grab_target;
	bool									m_is_target_escaped;
	bool									m_is_allow_stealth_kill;
	bool									m_on_stealth_kill;
	bool									m_is_forward_walk;
	bool									m_is_backward_walk;

	friend void from_json(const nlohmann::json& j_data, Zombie& zombie);
	friend void to_json  (nlohmann::json& j_data, const Zombie& zombie);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Zombie& zombie)
{
	from_json(j_data, static_cast<EnemyBase&>(zombie));

	j_data.at("data")			.get_to(zombie.data);
	j_data.at("arm_ray_data")	.get_to(zombie.m_arm_ray_data);
	j_data.at("leg_ray_data")	.get_to(zombie.m_leg_ray_data);
}

inline void to_json(nlohmann::json& j_data, const Zombie& zombie)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const EnemyBase&>(zombie));

	nlohmann::json derived_json =
	{
		{ "data",			zombie.data },
		{ "arm_ray_data",	zombie.m_arm_ray_data },
		{ "leg_ray_data",	zombie.m_leg_ray_data },
	};

	j_data = base_json;
	j_data.update(derived_json);
}
#pragma endregion
