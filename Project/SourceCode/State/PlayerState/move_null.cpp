#include "move_null.hpp"

player_state::MoveNull::MoveNull() :
	m_non_move_time(0.0f)
{

}

player_state::MoveNull::~MoveNull()
{

}

void player_state::MoveNull::Update(const Player* obj)
{
	m_non_move_time += FPS::GetDeltaTime();
}

void player_state::MoveNull::Enter(const Player* obj)
{
	m_non_move_time = 0.0f;
}

std::shared_ptr<IState<Player>> player_state::MoveNull::ChangeState(const Player* obj)
{
	if (obj->IsInputMove())
	{
		return std::make_shared<Move>();
	}

	return nullptr;
}
