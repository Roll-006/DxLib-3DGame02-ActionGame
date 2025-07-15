#include "turn_around.hpp"

player_state::TurnAround::TurnAround()
{

}

player_state::TurnAround::~TurnAround()
{

}

void player_state::TurnAround::Update(const Player* obj)
{

}

void player_state::TurnAround::Enter(const Player* obj)
{

}

IState<Player>* player_state::TurnAround::ChangeState(const Player* obj)
{
	auto test = TurnAround();
	return &test;
}
