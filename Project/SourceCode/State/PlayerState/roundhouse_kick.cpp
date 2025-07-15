#include "roundhouse_kick.hpp"

player_state::RoundhouseKick::RoundhouseKick()
{

}

player_state::RoundhouseKick::~RoundhouseKick()
{

}

void player_state::RoundhouseKick::Update(const Player* obj)
{

}

void player_state::RoundhouseKick::Enter(const Player* obj)
{

}

IState<Player>* player_state::RoundhouseKick::ChangeState(const Player* obj)
{
	auto test = RoundhouseKick();
	return &test;
}
