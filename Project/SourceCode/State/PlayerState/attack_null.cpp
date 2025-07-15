#include "attack_null.hpp"

player_state::AttackNull::AttackNull()
{

}

player_state::AttackNull::~AttackNull()
{

}

void player_state::AttackNull::Update(const Player* obj)
{

}

void player_state::AttackNull::Enter(const Player* obj)
{

}

IState<Player>* player_state::AttackNull::ChangeState(const Player* obj)
{
	auto test = AttackNull();
	return &test;
}
