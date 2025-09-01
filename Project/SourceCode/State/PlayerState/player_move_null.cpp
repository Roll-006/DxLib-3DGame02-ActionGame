#include "player_move_null.hpp"

player_state::MoveNull::MoveNull() : 
	MoveStateBase	(static_cast<int>(player_state::MoveStateKind::kMoveNull)),
	m_non_move_time	(0.0f)
{

}

player_state::MoveNull::~MoveNull()
{

}

void player_state::MoveNull::Update(std::shared_ptr<Player> obj)
{
	obj->CalcMoveSpeedStop();

	//m_non_move_time += FPS::GetDeltaTime();
}

void player_state::MoveNull::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::MoveNull::Enter(std::shared_ptr<Player> obj)
{
	//m_non_move_time = 0.0f;
}

void player_state::MoveNull::Exit(std::shared_ptr<Player> obj)
{
	
}

std::shared_ptr<IState<Player>> player_state::MoveNull::ChangeState(std::shared_ptr<Player> obj)
{
	// “ü—Í‚ª‚ ‚Á‚½ê‡AMove‚ÖˆÚs
	if (obj->GetStateController()->TryMove())
	{
		return obj->GetStateController()->GetState<Move, Player>();
	}

	return nullptr;
}
