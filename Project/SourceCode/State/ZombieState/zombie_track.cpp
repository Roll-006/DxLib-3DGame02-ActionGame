#include "../../Object/zombie.hpp"
#include "../../Kind/zombie_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/zombie_state_kind.hpp"
#include "zombie_state.hpp"
#include "zombie_track.hpp"

zombie_state::Track::Track(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator) :
	ZombieStateBase	(zombie, state, animator, ZombieStateKind::kTrack),
	m_in_sight_timer(0.0f)
{

}

zombie_state::Track::~Track()
{

}

void zombie_state::Track::Update()
{
	const auto target_pos = m_zombie.GetTarget()->GetTransform()->GetPos(CoordinateKind::kWorld);

	m_zombie.TrackMove(target_pos, true);

	if (m_zombie.GetMoveDir(TimeKind::kNext) != v3d::GetZeroV())
	{
		Move();
	}
	else
	{
		m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kIdle));

		m_zombie.InitMoveOffset();
	}

	if (m_zombie.CanAction()) { m_zombie.CalcAttackIntervalTime(); }
}

void zombie_state::Track::LateUpdate()
{
	m_zombie.OnFootIK();
}

void zombie_state::Track::Enter()
{
	m_in_sight_timer = 0.0f;
}

void zombie_state::Track::Exit()
{

}

const ZombieStateKind zombie_state::Track::GetNextStateKind()
{
	if (m_zombie.GetDeltaTime() <= 0.0f)
	{
		return ZombieStateKind::kNone;
	}
	// 強制待機
	else if (m_state.TryWaitForcibly())
	{
		return ZombieStateKind::kIdle;
	}
	// ダッシュ攻撃
	else if (m_zombie.CanAttack() && m_state.TryGrabRun())
	{
		return ZombieStateKind::kGrabRun;
	}
	// 待機
	else if (!m_state.TryTrack() && !m_state.TryGrabRun())
	{
		return ZombieStateKind::kIdle;
	}
	// ステルスキル
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