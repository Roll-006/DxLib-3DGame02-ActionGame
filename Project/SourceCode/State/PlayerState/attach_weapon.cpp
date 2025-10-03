#include "attach_weapon.hpp"

player_state::AttachWeapon::AttachWeapon() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kAttachWeapon)),
	m_is_all_stop			(false)
{

}

player_state::AttachWeapon::~AttachWeapon()
{

}

void player_state::AttachWeapon::Update(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AttachWeapon::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::AttachWeapon::Enter(std::shared_ptr<Player>& obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::AttachWeapon::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::AttachWeapon::ChangeState(std::shared_ptr<Player>& obj)
{
	return nullptr;
}
