#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "../../Event/event_system.hpp"
#include "zombie_state.hpp"
#include "zombie_stand_stun.hpp"

zombie_state::StandStun::StandStun(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase	(zombie, state, animator, ZombieStateKind::kStandStun),
	m_stun_timer	(0.0f)
{

}

zombie_state::StandStun::~StandStun()
{

}

void zombie_state::StandStun::Update()
{
	m_zombie.CalcMoveSpeedStop();
	m_zombie.InitMoveOffset();

	if (m_zombie.CanAction())
	{
		m_stun_timer += m_zombie.GetDeltaTime();
	}

	m_zombie.DisallowDecreaseKnockBackGauge();

	m_zombie.UpdateLocomotion();

	m_animator->AttachAnim(static_cast<int>(ZombieAnimKind::kIdle),		 Animator::BodyKind::kLowerBody);
	m_animator->AttachAnim(static_cast<int>(ZombieAnimKind::kStandStun), Animator::BodyKind::kUpperBody);
}

void zombie_state::StandStun::LateUpdate()
{
	m_zombie.OnFootIK();
}

void zombie_state::StandStun::Enter()
{
	m_stun_timer = 0.0f;

	m_zombie.GetHealth(HealthPartKind::kHead)->IncreaseCurrentMax();

	const auto model_handle = m_zombie.GetModeler()->GetModelHandle();
	auto	   head_m = MV1GetFrameLocalWorldMatrix(
		model_handle,
		m_zombie.GetHumanoidFrame()->GetHeadIndex(model_handle)
	);
	const auto head_pos = matrix::GetPos(head_m);

	EventSystem::GetInstance()->Publish(StunEvent(head_pos));
}

void zombie_state::StandStun::Exit()
{
	const auto model_handle = m_zombie.GetModeler()->GetModelHandle();
	auto	   head_m = MV1GetFrameLocalWorldMatrix(
		model_handle,
		m_zombie.GetHumanoidFrame()->GetHeadIndex(model_handle)
	);
	const auto head_pos = matrix::GetPos(head_m);

	EventSystem::GetInstance()->Publish(ExitStunEvent(head_pos));
}

const ZombieStateKind zombie_state::StandStun::GetNextStateKind()
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
	// 左足ダウン
	else if (m_state.TryLeftCrouchStun())
	{
		return ZombieStateKind::kCrouchLeftStun;
	}
	// 右足ダウン
	else if (m_state.TryRightCrouchStun())
	{
		return ZombieStateKind::kCrouchRightStun;
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