#include "stab_knife.hpp"

player_state::StabKnife::StabKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kStabKnife))
{

}

player_state::StabKnife::~StabKnife()
{

}

void player_state::StabKnife::Update(Player* obj)
{

}

void player_state::StabKnife::Enter(Player* obj)
{

}

void player_state::StabKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::StabKnife::ChangeState(const Player* obj)
{
	return nullptr;
}
