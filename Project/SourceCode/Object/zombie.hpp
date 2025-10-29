#pragma once
#include "../Base/enemy_base.hpp"
#include "../Interface/i_melee_hittable.hpp"
#include "../Interface/i_grabber.hpp"
#include "../Interface/i_stealth_killable.hpp"

#include "../Part/zombie_animator.hpp"

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

	[[nodiscard]] float GetDamageOverTimeStartTime()	const override	{ return kDamageOverTimeStartTime; }
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
	[[nodiscard]] bool										CanGrabTraget()			const	{ return m_can_grab_target; }
	#pragma endregion

private:
	void JudgeAction() override;

private:
	static constexpr VECTOR kBasicAngle				= { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale				= 0.3f;

	static constexpr float kFOV						= 80.0f;
	static constexpr float kVisibleDistance			= 300.0f;

	static constexpr float kAttackIntervalTime		= 3.0f;
	static constexpr float kInvincibleTime			= 0.5f;

	static constexpr float kWalkSpeed				= 60.0f;
	static constexpr float kRunSpeed				= 100.0f;
	static constexpr float kRunGrabSpeed			= 110.0f;

	static constexpr float kMoveDirOffsetSpeed		= 5.0f;			// 移動方向の補正速度
	static constexpr float kLookDirOffsetSpeed		= 4.0f;			// 見る方向の補正角度

	static constexpr float  kCapsuleRadius			= 8.0f;
	static constexpr float  kCollisionAreaRadius	= 40.0f;
	static constexpr VECTOR kCollisionAreaOffset	= { 0.0f, 30.0f, 0.0f };
	static constexpr float  kLandingTriggerRadius	= 6.0f;
	static constexpr float  kHeadTriggerRadius		= 5.0f;
	static constexpr float  kUpBodyTriggerRadius	= 6.0f;
	static constexpr float  kDownBodyTriggerRadius	= 6.0f;
	static constexpr float  kUpperArmTriggerRadius	= 2.5f;
	static constexpr float  kForearmTriggerRadius	= 2.2f;
	static constexpr float  kHandTriggerRadius		= 2.0f;
	static constexpr float  kUpLegTriggerRadius		= 3.5f;
	static constexpr float  kDownLegTriggerRadius	= 2.5f;

	static constexpr float kDamageOverTimeStartTime = 1.0f;

private:
	std::shared_ptr<ZombieStateController> m_state;

	bool m_can_grab_target;
	bool m_is_target_escaped;
	bool m_is_allow_stealth_kill;
	bool m_on_stealth_kill;
};
