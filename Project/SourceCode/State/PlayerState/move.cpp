#include "move.hpp"

player_state::Move::Move()
{

}

player_state::Move::~Move()
{

}

void player_state::Move::Update(const Player* obj)
{

}

void player_state::Move::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Move::ChangeState(const Player* obj)
{
	if (!obj->IsInputMove())
	{
		return std::make_shared<MoveNull>();
	}

	return nullptr;
}
