#include "player_move.hpp"

player_state::Move::Move() :
	MoveStateBase			(static_cast<int>(player_state::MoveStateKind::kMove)),
	m_is_first_move_frame	(false),
	m_is_all_stop			(false)
{

}

player_state::Move::~Move()
{

}

void player_state::Move::Update(std::shared_ptr<Player> obj)
{
	obj->Move();

	m_is_first_move_frame = false;
}

void player_state::Move::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::Move::Enter(std::shared_ptr<Player> obj)
{
	m_is_first_move_frame	= true;
}

void player_state::Move::Exit(std::shared_ptr<Player> obj)
{
	
}

std::shared_ptr<IState<Player>> player_state::Move::ChangeState(std::shared_ptr<Player> obj)
{
	// IDLE
	if (!obj->GetStateController()->TryMove())
	{
		return obj->GetStateController()->GetState<Idle, Player>();
	}

	return nullptr;
}
