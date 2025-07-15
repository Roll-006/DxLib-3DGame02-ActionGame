#include "squat.hpp"

player_state::Squat::Squat()
{

}

player_state::Squat::~Squat()
{

}

void player_state::Squat::Update(const Player* obj)
{

}

void player_state::Squat::Enter(const Player* obj)
{

}

IState<Player>* player_state::Squat::ChangeState(const Player* obj)
{
	auto test = Squat();
	return &test;
}
