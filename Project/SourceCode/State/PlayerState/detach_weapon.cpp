#include "detach_weapon.hpp"

player_state::DetachWeapon::DetachWeapon() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kStabKnife))
{

}

player_state::DetachWeapon::~DetachWeapon()
{

}

void player_state::DetachWeapon::Update(Player* obj)
{
	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::DetachWeapon::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::DetachWeapon::Enter(Player* obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::DetachWeapon::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::DetachWeapon::ChangeState(Player* obj)
{
	return nullptr;
}
