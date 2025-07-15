#include "escape.hpp"

player_state::Escape::Escape()
{

}

player_state::Escape::~Escape()
{

}

void player_state::Escape::Update(const Player* obj)
{

}

void player_state::Escape::Enter(const Player* obj)
{

}

IState<Player>* player_state::Escape::ChangeState(const Player* obj)
{
	auto test = Escape();
	return &test;
}
