#include "boss_move.hpp"

boss_state::Move::Move() :
	MoveStateBase		(static_cast<int>(boss_state::MoveStateKind::kMove)),
	m_is_stop_all_state	(false)
{

}

boss_state::Move::~Move()
{

}

void boss_state::Move::Update(std::shared_ptr<Boss>& obj)
{
	const auto state_controller = obj->GetStateController();
	const auto ai_state			= static_cast<boss_state::AIStateKind>(state_controller->GetAIState(TimeKind::kCurrent)->GetStateKind());
	const auto target_pos		= state_controller->GetTargetCharacter()->GetTransform()->GetPos(CoordinateKind::kWorld);

	obj->CalcMoveSpeed();
	obj->Move();

	if (   ai_state == boss_state::AIStateKind::kTrack
		|| ai_state == boss_state::AIStateKind::kRunAttack)
	{
		obj->TrackMove(target_pos);
	}
	else if (ai_state == boss_state::AIStateKind::kPatrol)
	{
		obj->ChangePatrolDestination();
	}
}

void boss_state::Move::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Move::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Move::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Move::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// IDLE
	if (!state_controller->TryMove())
	{
		return state_controller->GetState<Idle, Boss>();
	}

	return nullptr;
}
