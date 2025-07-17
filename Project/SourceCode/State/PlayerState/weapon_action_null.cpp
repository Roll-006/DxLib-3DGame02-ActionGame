#include "weapon_action_null.hpp"

player_state::WeaponActionNull::WeaponActionNull() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull))
{

}

player_state::WeaponActionNull::~WeaponActionNull()
{

}

void player_state::WeaponActionNull::Update(Player* obj)
{

}

void player_state::WeaponActionNull::Enter(Player* obj)
{

}

void player_state::WeaponActionNull::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::WeaponActionNull::ChangeState(const Player* obj)
{
	return nullptr;
}
