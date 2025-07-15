#include "action_null.hpp"

player_state::ActionNull::ActionNull()
{

}

player_state::ActionNull::~ActionNull()
{

}

void player_state::ActionNull::Update(const Player* obj)
{

}

void player_state::ActionNull::Enter(const Player* obj)
{

}

IState<Player>* player_state::ActionNull::ChangeState(const Player* obj)
{
	auto test = ActionNull();
	return &test;
}
