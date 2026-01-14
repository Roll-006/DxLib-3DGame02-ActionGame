#include <memory>
#include <unordered_map>
#include "../../Object/zombie.hpp"
#include "../../Animator/animator.hpp"
#include "../../Base/zombie_state_base.hpp"
#include "../../Kind/zombie_state_kind.hpp"

#include "zombie_idle.hpp"
#include "zombie_dead.hpp"
#include "zombie_patrol.hpp"
#include "zombie_track.hpp"
#include "zombie_search.hpp"
#include "zombie_stand_up.hpp"
#include "zombie_stand_stun.hpp"
#include "zombie_crouch_left_stun.hpp"
#include "zombie_crouch_right_stun.hpp"
#include "zombie_stealth_killed.hpp"
#include "zombie_detected.hpp"
#include "zombie_grab.hpp"
#include "zombie_grab_run.hpp"
#include "zombie_knockback.hpp"
#include "zombie_backward_knockback.hpp"

#include "zombie_state.hpp"

zombie_state::State::State(Zombie& zombie, const std::shared_ptr<Animator>& animator):
	m_zombie			(zombie),
	m_current_state		(nullptr),
	m_prev_state_kind	(ZombieStateKind::kIdle)
{
	m_states[ZombieStateKind::kIdle]				= std::make_shared<zombie_state::Idle>				(m_zombie, *this, animator);
	m_states[ZombieStateKind::kDead]				= std::make_shared<zombie_state::Dead>				(m_zombie, *this, animator);
	m_states[ZombieStateKind::kPatrol]				= std::make_shared<zombie_state::Patrol>			(m_zombie, *this, animator);
	m_states[ZombieStateKind::kTrack]				= std::make_shared<zombie_state::Track>				(m_zombie, *this, animator);
	m_states[ZombieStateKind::kSearch]				= std::make_shared<zombie_state::Search>			(m_zombie, *this, animator);
	m_states[ZombieStateKind::kStandUp]				= std::make_shared<zombie_state::StandUp>			(m_zombie, *this, animator);
	m_states[ZombieStateKind::kStandStun]			= std::make_shared<zombie_state::StandStun>			(m_zombie, *this, animator);
	m_states[ZombieStateKind::kCrouchLeftStun]		= std::make_shared<zombie_state::CrouchLeftStun>	(m_zombie, *this, animator);
	m_states[ZombieStateKind::kCrouchRightStun]		= std::make_shared<zombie_state::CrouchRightStun>	(m_zombie, *this, animator);
	m_states[ZombieStateKind::kStealthKilled]		= std::make_shared<zombie_state::StealthKilled>		(m_zombie, *this, animator);
	m_states[ZombieStateKind::kDetected]			= std::make_shared<zombie_state::Detected>			(m_zombie, *this, animator);
	m_states[ZombieStateKind::kGrab]				= std::make_shared<zombie_state::Grab>				(m_zombie, *this, animator);
	m_states[ZombieStateKind::kGrabRun]				= std::make_shared<zombie_state::GrabRun>			(m_zombie, *this, animator);
	m_states[ZombieStateKind::kKnockback]			= std::make_shared<zombie_state::Knockback>			(m_zombie, *this, animator);
	m_states[ZombieStateKind::kBackwardKnockback]	= std::make_shared<zombie_state::BackwardKnockback>	(m_zombie, *this, animator);

	m_current_state = m_states.at(ZombieStateKind::kIdle);
	m_current_state->Enter();
}

zombie_state::State::~State()
{

}

void zombie_state::State::Update()
{
	// 次のステートがNoneでない場合はステートを変更
	const auto next_state_kind = m_current_state->GetNextStateKind();
	if (next_state_kind != ZombieStateKind::kNone)
	{
		m_prev_state_kind = m_current_state->GetStateKind();
		m_current_state->Exit();
		m_current_state = m_states.at(next_state_kind);
		m_current_state->Enter();
	}

	m_current_state->Update();
}

void zombie_state::State::LateUpdate()
{
	m_current_state->LateUpdate();
}

const ZombieStateKind zombie_state::State::GetPrevStateKind() const
{
	return m_prev_state_kind;
}

const ZombieStateKind zombie_state::State::GetCurrentStateKind() const
{
	return m_current_state->GetStateKind();
}


#pragma region Try判定
bool zombie_state::State::TryWaitForcibly()
{
	return !m_zombie.CanAction();
}

bool zombie_state::State::TryPatrol()
{
	const auto route_giver = m_zombie.GetPatrolRouteGiver();

	if (!route_giver)					{ return false; }
	if (route_giver->IsEnd())			{ return false; }
	if (m_zombie.IsDetectedTarget())	{ return false; }

	return true;
}

bool zombie_state::State::TryTrack()
{
	if (!m_zombie.GetTarget())			{ return false; }
	if (!m_zombie.IsDetectedTarget())	{ return false; }

	const auto is_in_sight = m_zombie.IsDetectedTarget();

	return is_in_sight;
}

bool zombie_state::State::TryRunAttack()
{
	if (!m_zombie.GetTarget()) { return false; }

	const auto is_in_sight = m_zombie.IsDetectedTarget();
	const auto can_attack = m_zombie.CanAttack();

	return is_in_sight && can_attack;
}

bool zombie_state::State::TryDetected()
{
	return m_zombie.IsDetectedTarget() && !m_zombie.IsPrevDetectedTarget();
}

bool zombie_state::State::TryWalk()
{
	if (!m_zombie.GetTarget()) { return false; }
	//if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() != static_cast<int>(zombie_state::MoveStateKind::kMove)) { return false; }

	const auto pos = m_zombie.GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos = m_zombie.GetTarget()->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto distance = VSize(pos - target_pos);

	return distance < 140.0f;
}

bool zombie_state::State::TryRun()
{
	if (!m_zombie.IsDetectedTarget()) { return false; }
	if (!m_zombie.GetTarget()) { return false; }
	//if (m_move_state.at(TimeKind::kCurrent)->GetStateKind() != static_cast<int>(zombie_state::MoveStateKind::kMove)) { return false; }

	const auto pos			= m_zombie.GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos	= m_zombie.GetTarget()->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto distance		= VSize(pos - target_pos);

	// TODO : 後に定数化
	return distance > 160.0f;
}

bool zombie_state::State::TryStealthKilled()
{
	return m_zombie.IsStealthKilled();
}

bool zombie_state::State::TryGrabRun()
{
	if (!m_zombie.IsDetectedTarget()) { return false; }

	const auto pos			= m_zombie.GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos	= m_zombie.GetTarget()->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto distance		= VSize(pos - target_pos);

	return distance < 90.0f;
}

bool zombie_state::State::TryKnockback()
{
	return m_zombie.GetKnockBackSpeed() > 0.0f;
}

bool zombie_state::State::TryBackwardKnockback()
{
	return !m_zombie.GetKnockBackGauge()->IsAlive();
}

bool zombie_state::State::TryDead()
{
	return m_zombie.GetHealth(HealthPartKind::kMain)->GetCurrentValue() <= 0.0f;
}

bool zombie_state::State::TryLeftCrouchStun()
{
	return m_zombie.GetHealth(HealthPartKind::kLeftLeg)->GetCurrentValue() <= 0.0f;
}

bool zombie_state::State::TryRightCrouchStun()
{
	return m_zombie.GetHealth(HealthPartKind::kRightLeg)->GetCurrentValue() <= 0.0f;
}

bool zombie_state::State::TryStandStun()
{
	return m_zombie.GetHealth(HealthPartKind::kHead)->GetCurrentValue() <= 0.0f;;
}
#pragma endregion
