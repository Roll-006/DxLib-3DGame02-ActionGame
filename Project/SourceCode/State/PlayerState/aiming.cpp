#include "aiming.hpp"

player_state::Aiming::Aiming() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAiming))
{

}

player_state::Aiming::~Aiming()
{

}

void player_state::Aiming::Update(Player* obj)
{

}

void player_state::Aiming::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Aiming::ChangeState(const Player* obj)
{
	return nullptr;
}
