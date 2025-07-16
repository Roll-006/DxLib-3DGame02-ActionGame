#include "action_null.hpp"

player_state::ActionNull::ActionNull()
{

}

player_state::ActionNull::~ActionNull()
{

}

void player_state::ActionNull::Update(Player* obj)
{

}

void player_state::ActionNull::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::ActionNull::ChangeState(const Player* obj)
{
	return nullptr;
}
