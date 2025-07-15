#include "suplex.hpp"

player_state::Suplex::Suplex()
{

}

player_state::Suplex::~Suplex()
{

}

void player_state::Suplex::Update(const Player* obj)
{

}

void player_state::Suplex::Enter(const Player* obj)
{

}

IState<Player>* player_state::Suplex::ChangeState(const Player* obj)
{
	auto test = Suplex();
	return &test;
}
