#include "idle.hpp"

player_state::Idle::Idle() :
	m_non_move_time(0.0f)
{

}

player_state::Idle::~Idle()
{

}

void player_state::Idle::Update(const Player* obj)
{

}

void player_state::Idle::Enter(const Player* obj)
{

}

IState<Player>* player_state::Idle::ChangeState(const Player* obj)
{
	auto test = Idle();
	return &test;
}
