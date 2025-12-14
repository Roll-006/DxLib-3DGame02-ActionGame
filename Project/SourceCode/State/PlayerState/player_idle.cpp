#include "player_idle.hpp"

player_state::Idle::Idle(Player& player) :
	MoveStateBase		(static_cast<int>(player_state::MoveStateKind::kIdle)),
	m_non_move_time		(0.0f),
	m_is_stop_all_state	(false),
	m_player			(player)
{

}

player_state::Idle::~Idle()
{

}

void player_state::Idle::Update()
{
	obj->CalcMoveSpeedStop();

	//m_non_move_time += FPS::GetDeltaTime();
}

void player_state::Idle::LateUpdate()
{
	
}

void player_state::Idle::Enter()
{
	//m_non_move_time = 0.0f;
}

void player_state::Idle::Exit()
{
	
}

int player_state::Idle::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	// 入力があった場合、Moveへ移行
	if (obj->GetStateController()->TryMove(obj))
	{
		return obj->GetStateController()->GetState<Move, Player>();
	}

	return nullptr;
}
