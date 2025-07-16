#include "weapon_action_null.hpp"

player_state::WeaponActionNull::WeaponActionNull()
{

}

player_state::WeaponActionNull::~WeaponActionNull()
{

}

void player_state::WeaponActionNull::Update(Player* obj)
{

}

void player_state::WeaponActionNull::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::WeaponActionNull::ChangeState(const Player* obj)
{
	return nullptr;
}
