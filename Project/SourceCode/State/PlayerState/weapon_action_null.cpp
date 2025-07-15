#include "weapon_action_null.hpp"

player_state::WeaponActionNull::WeaponActionNull()
{

}

player_state::WeaponActionNull::~WeaponActionNull()
{

}

void player_state::WeaponActionNull::Update(const Player* obj)
{

}

void player_state::WeaponActionNull::Enter(const Player* obj)
{

}

IState<Player>* player_state::WeaponActionNull::ChangeState(const Player* obj)
{
	auto test = WeaponActionNull();
	return &test;
}
