#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Interface/i_humanoid.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "zombie_state.hpp"
#include "zombie_crouch_left_stun.hpp"

zombie_state::CrouchLeftStun::CrouchLeftStun(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kCrouchLeftStun),
	m_stun_timer(0.0f)
{

}

zombie_state::CrouchLeftStun::~CrouchLeftStun()
{

}

void zombie_state::CrouchLeftStun::Update()
{
	m_zombie.CalcMoveSpeedStop();
	m_zombie.InitMoveOffset();

	if (m_zombie.CanAction())
	{
		m_stun_timer += m_zombie.GetDeltaTime();
	}

	m_zombie.DisallowDecreaseKnockBackGauge();
	m_zombie.AllowCalcLookDir();

	m_zombie.UpdateLocomotion();

	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kLeftCrouch));
}

void zombie_state::CrouchLeftStun::LateUpdate()
{
	m_zombie.OnLeftCrouchIK();
}

void zombie_state::CrouchLeftStun::Enter()
{
	m_stun_timer = 0.0f;

	// 左脚耐久回復
	m_zombie.GetHealth(HealthPartKind::kLeftLeg)->IncreaseCurrentMax();

	// スタン演出
	const auto model_handle = m_zombie.GetModeler()->GetModelHandle();
	auto head_m = MV1GetFrameLocalWorldMatrix(
		model_handle,
		m_zombie.GetHumanoidFrame()->GetHeadIndex(model_handle)
	);
	const auto head_pos = matrix::GetPos(head_m);

	EventSystem::GetInstance()->Publish(StunEvent(head_pos));

	// 被弾反動回転
	m_zombie.OnRotate(30.0f * math::kDegToRad, RotDirKind::kLeft);

	// IK Ray生成
	const auto& humanoid = *dynamic_cast<IHumanoid*>(&m_zombie);
	m_zombie.GetHumanoidFootIKSolver()->CreateLeftLegRay (&m_zombie);
	m_zombie.GetHumanoidArmIKSolver ()->CreateLeftHandRay(&m_zombie);
}

void zombie_state::CrouchLeftStun::Exit()
{
	// スタン解除演出
	const auto model_handle = m_zombie.GetModeler()->GetModelHandle();
	const auto head_m = MV1GetFrameLocalWorldMatrix(model_handle, m_zombie.GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos = matrix::GetPos(head_m);

	EventSystem::GetInstance()->Publish(ExitStunEvent(head_pos));

	// IK Ray削除
	m_zombie.GetHumanoidFootIKSolver()->DeleteLeftLegRay (&m_zombie);
	m_zombie.GetHumanoidArmIKSolver ()->DeleteLeftHandRay(&m_zombie);
}

const ZombieStateKind zombie_state::CrouchLeftStun::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}
	// ステルスキルされた
	else if (m_state.TryStealthKilled())
	{
		return ZombieStateKind::kStealthKilled;
	}
	// 死亡
	else if (m_state.TryDead())
	{
		return ZombieStateKind::kDead;
	}
	// ノックバック
	else if (m_state.TryKnockback())
	{
		return ZombieStateKind::kKnockback;
	}
	// IDLE
	else if (m_stun_timer > kStunTime)
	{
		return ZombieStateKind::kIdle;
	}

	return ZombieStateKind::kNone;
}
