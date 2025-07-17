#include "aim_gun.hpp"

player_state::AimGun::AimGun() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAimGun))
{

}

player_state::AimGun::~AimGun()
{

}

void player_state::AimGun::Update(Player* obj)
{

}

void player_state::AimGun::Enter(Player* obj)
{

}

void player_state::AimGun::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::AimGun::ChangeState(const Player* obj)
{
	return nullptr;
}
