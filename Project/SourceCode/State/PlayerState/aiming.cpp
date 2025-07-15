#include "aiming.hpp"

player_state::Aiming::Aiming()
{

}

player_state::Aiming::~Aiming()
{

}

void player_state::Aiming::Update(const Player* obj)
{

}

void player_state::Aiming::Enter(const Player* obj)
{

}

IState<Player>* player_state::Aiming::ChangeState(const Player* obj)
{
	auto test = Aiming();
	return &test;
}
