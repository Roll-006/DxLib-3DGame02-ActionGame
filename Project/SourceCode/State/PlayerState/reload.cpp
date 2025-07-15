#include "reload.hpp"

player_state::Reload::Reload()
{

}

player_state::Reload::~Reload()
{

}

void player_state::Reload::Update(const Player* obj)
{

}

void player_state::Reload::Enter(const Player* obj)
{

}

IState<Player>* player_state::Reload::ChangeState(const Player* obj)
{
	auto test = Reload();
	return &test;
}
