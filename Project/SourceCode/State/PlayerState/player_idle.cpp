#include "player_idle.hpp"

player_state::Idle::Idle() : 
	MoveStateBase		(static_cast<int>(player_state::MoveStateKind::kIdle)),
	m_non_move_time		(0.0f),
	m_is_stop_all_state	(false)
{

}

player_state::Idle::~Idle()
{

}

void player_state::Idle::Update(std::shared_ptr<Player>& obj)
{
	obj->CalcMoveSpeedStop();

	//m_non_move_time += FPS::GetDeltaTime();
}

void player_state::Idle::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Idle::Enter(std::shared_ptr<Player>& obj)
{
	//m_non_move_time = 0.0f;
}

void player_state::Idle::Exit(std::shared_ptr<Player>& obj)
{
	
}

std::shared_ptr<IState<Player>> player_state::Idle::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	// “ü—Í‚ª‚ ‚Á‚½ê‡AMove‚ÖˆÚs
	if (obj->GetStateController()->TryMove())
	{
		return obj->GetStateController()->GetState<Move, Player>();
	}

	return nullptr;
}
