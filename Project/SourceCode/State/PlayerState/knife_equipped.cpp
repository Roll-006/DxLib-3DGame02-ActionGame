#include "knife_equipped.hpp"

player_state::KnifeEquipped::KnifeEquipped() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped))
{

}

player_state::KnifeEquipped::~KnifeEquipped()
{

}

void player_state::KnifeEquipped::Update(Player* obj)
{

}

void player_state::KnifeEquipped::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::KnifeEquipped::ChangeState(const Player* obj)
{
	return nullptr;
}
