#include "detach_weapon.hpp"

player_state::DetachWeapon::DetachWeapon() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kStabKnife)),
	m_is_all_stop			(false)
{

}

player_state::DetachWeapon::~DetachWeapon()
{

}

void player_state::DetachWeapon::Update(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::DetachWeapon::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::DetachWeapon::Enter(std::shared_ptr<Player>& obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::DetachWeapon::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::DetachWeapon::ChangeState(std::shared_ptr<Player>& obj)
{
	return nullptr;
}
