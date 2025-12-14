#include "player_move.hpp"

player_state::Move::Move(Player& player) :
	MoveStateBase			(static_cast<int>(player_state::MoveStateKind::kMove)),
	m_is_first_move_frame	(false),
	m_is_stop_all_state		(false),
	m_player				(player)
{

}

player_state::Move::~Move()
{

}

void player_state::Move::Update()
{
	obj->Move();

	m_is_first_move_frame = false;
}

void player_state::Move::LateUpdate()
{
	
}

void player_state::Move::Enter()
{
	m_is_first_move_frame	= true;
}

void player_state::Move::Exit()
{
	
}

int player_state::Move::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	// IDLE
	if (!obj->GetStateController()->TryMove(obj))
	{
		return obj->GetStateController()->GetState<Idle, Player>();
	}

	return nullptr;
}
