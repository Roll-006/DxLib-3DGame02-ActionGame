#include "aim_knife.hpp"

player_state::AimKnife::AimKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAimKnife))
{

}

player_state::AimKnife::~AimKnife()
{

}

void player_state::AimKnife::Update(Player* obj)
{

}

void player_state::AimKnife::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::AimKnife::ChangeState(const Player* obj)
{
	return nullptr;
}
