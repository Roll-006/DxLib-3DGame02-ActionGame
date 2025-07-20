#include "equip_knife.hpp"

player_state::EquipKnife::EquipKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife))
{

}

player_state::EquipKnife::~EquipKnife()
{

}

void player_state::EquipKnife::Update(Player* obj)
{

}

void player_state::EquipKnife::LateUpdate(Player* obj)
{

}

void player_state::EquipKnife::Enter(Player* obj)
{

}

void player_state::EquipKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::EquipKnife::ChangeState(const Player* obj)
{
	return nullptr;
}
