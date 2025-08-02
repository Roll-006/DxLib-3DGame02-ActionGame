#include "parry.hpp"

player_state::Parry::Parry() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kParry))
{

}

player_state::Parry::~Parry()
{

}

void player_state::Parry::Update(Player* obj)
{

}

void player_state::Parry::LateUpdate(Player* obj)
{

}

void player_state::Parry::Enter(Player* obj)
{

}

void player_state::Parry::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Parry::ChangeState(Player* obj)
{
	return nullptr;
}
