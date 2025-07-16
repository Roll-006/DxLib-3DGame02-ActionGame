#include "gun_equipped.hpp"

player_state::GunEquipped::GunEquipped() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped))
{

}

player_state::GunEquipped::~GunEquipped()
{

}

void player_state::GunEquipped::Update(Player* obj)
{

}

void player_state::GunEquipped::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::GunEquipped::ChangeState(const Player* obj)
{
	return nullptr;
}
