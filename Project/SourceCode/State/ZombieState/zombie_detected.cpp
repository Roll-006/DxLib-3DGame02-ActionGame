#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "zombie_state.hpp"
#include "zombie_detected.hpp"

zombie_state::Detected::Detected(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase(zombie, state, animator, ZombieStateKind::kDetected)
{

}

zombie_state::Detected::~Detected()
{

}

void zombie_state::Detected::Update()
{
	m_zombie.Detected();
	m_zombie.CalcMoveSpeedStop();
	m_zombie.InitMoveOffset();

	m_zombie.UpdateLocomotion();

	m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kDetected));
}

void zombie_state::Detected::LateUpdate()
{
	m_zombie.OnFootIK();
}

void zombie_state::Detected::Enter()
{
	const auto pos = m_zombie.GetTransform()->GetPos(CoordinateKind::kWorld);

	EventSystem::GetInstance()->Publish(OnTargetDetectedEvent(m_zombie.GetEnemyID(), m_zombie.GetDetecteNotifyDistance(), pos));
}

void zombie_state::Detected::Exit()
{

}

const ZombieStateKind zombie_state::Detected::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}
	else if (!m_animator->IsPlayEnd(Animator::BodyKind::kUpperBody))
	{
		return ZombieStateKind::kNone;
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
	// 強制待機
	else if (m_state.TryWaitForcibly())
	{
		return ZombieStateKind::kIdle;
	}
	// ステルスキル
	else if (m_state.TryStealthKilled())
	{
		return ZombieStateKind::kStealthKilled;
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

	return ZombieStateKind::kNone;
}
