#include "zombie_move.hpp"

zombie_state::Move::Move() :
	MoveStateBase		(static_cast<int>(zombie_state::MoveStateKind::kMove)),
	m_is_stop_all_state	(false)
{

}

zombie_state::Move::~Move()
{

}

void zombie_state::Move::Update(std::shared_ptr<Zombie>& obj)
{
	const auto state_controller = obj->GetStateController();
	const auto ai_state			= static_cast<zombie_state::AIStateKind>(state_controller->GetAIState(TimeKind::kCurrent)->GetStateKind());
	const auto target_pos		= state_controller->GetTargetCharacter()->GetTransform()->GetPos(CoordinateKind::kWorld);

	obj->CalcMoveSpeed();
	obj->Move();

	if (   ai_state == zombie_state::AIStateKind::kTrack
		|| ai_state == zombie_state::AIStateKind::kRunAttack)
	{
		obj->TrackMove(target_pos);
	}
	else if (ai_state == zombie_state::AIStateKind::kPatrol)
	{
		obj->ChangePatrolDestination();
	}
}

void zombie_state::Move::LateUpdate(std::shared_ptr<Zombie>& obj)
{
	
}

void zombie_state::Move::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Move::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Move::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// IDLE
	if (!state_controller->TryMove())
	{
		return state_controller->GetState<Idle, Zombie>();
	}

	return nullptr;
}
