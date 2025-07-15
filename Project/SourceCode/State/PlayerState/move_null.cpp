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

}

void player_state::MoveNull::Enter(const Player* obj)
{

}

IState<Player>* player_state::MoveNull::ChangeState(const Player* obj)
{
	auto test = MoveNull();
	return &test;
}
