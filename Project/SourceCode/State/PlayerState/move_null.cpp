#include "move_null.hpp"

player_state::MoveNull::MoveNull() : 
	MoveStateBase	(static_cast<int>(player_state::MoveStateKind::kMoveNull)),
	m_non_move_time	(0.0f)
{

}

player_state::MoveNull::~MoveNull()
{

}

void player_state::MoveNull::Update(Player* obj)
{
	m_non_move_time += FPS::GetDeltaTime();
}

void player_state::MoveNull::Enter(const Player* obj)
{
	m_non_move_time = 0.0f;
}

std::shared_ptr<IState<Player>> player_state::MoveNull::ChangeState(const Player* obj)
{
	// “ü—Í‚ª‚ ‚Á‚½ê‡AMove‚ÖˆÚs
	if (obj->GetStateController()->TryMove())
	{
		return obj->GetStateController()->GetState<Move, Player>();
	}

	return nullptr;
}
