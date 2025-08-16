#include "attach_weapon.hpp"

player_state::AttachWeapon::AttachWeapon() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAttachWeapon))
{

}

player_state::AttachWeapon::~AttachWeapon()
{

}

void player_state::AttachWeapon::Update(Player* obj)
{
	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AttachWeapon::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::AttachWeapon::Enter(Player* obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::AttachWeapon::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::AttachWeapon::ChangeState(Player* obj)
{
	return nullptr;
}
