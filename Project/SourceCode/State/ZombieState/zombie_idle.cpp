#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "zombie_state.hpp"
#include "zombie_idle.hpp"

zombie_state::Idle::Idle(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kIdle)
{

}

zombie_state::Idle::~Idle()
{

}

void zombie_state::Idle::Update()
{
	m_zombie.CalcMoveSpeedStop();
	m_zombie.InitMoveOffset();

	m_zombie.UpdateLocomotion();

	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kIdle));
}

void zombie_state::Idle::LateUpdate()
{
	m_zombie.OnFootIK();
}

void zombie_state::Idle::Enter()
{
	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kIdle));
}

void zombie_state::Idle::Exit()
{

}

const ZombieStateKind zombie_state::Idle::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}
	// 強制待機
	else if (m_state.TryWaitForcibly())
	{
		return ZombieStateKind::kNone;
	}
	// 発見
	else if (m_state.TryDetected())
	{
		return ZombieStateKind::kDetected;
	}
	// 巡回
	else if (m_state.TryPatrol())
	{
		return ZombieStateKind::kPatrol;
	}
	// 追跡
	else if (m_state.TryTrack())
	{
		return ZombieStateKind::kTrack;
	}
	// ステルスキル
	else if (m_state.TryStealthKilled())
	{
		return ZombieStateKind::kStealthKilled;
	}
	// 死亡
	if (m_state.TryDead())
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
	// 立ちダウン
	else if (m_state.TryStandStun())
	{
		return ZombieStateKind::kStandStun;
	}
	// ノックバック
	else if (m_state.TryKnockback())
	{
		return ZombieStateKind::kKnockback;
	}
	// ノックバック（後ろ）
	else if (m_state.TryBackwardKnockback())
	{
		m_zombie.OnKnockback(-m_zombie.GetLookDir(TimeKind::kCurrent), 70.0f, 60.0f);
		return ZombieStateKind::kBackwardKnockback;
	}

	return ZombieStateKind::kNone;
}
