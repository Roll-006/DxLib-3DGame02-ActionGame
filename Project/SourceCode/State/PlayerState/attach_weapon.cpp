#include "attach_weapon.hpp"

player_state::AttachWeapon::AttachWeapon(Player& player) :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kAttachWeapon)),
	m_is_stop_all_state		(false),
	m_player				(player)
{

}

player_state::AttachWeapon::~AttachWeapon()
{

}

void player_state::AttachWeapon::Update()
{
	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AttachWeapon::LateUpdate()
{
	
}

void player_state::AttachWeapon::Enter()
{
	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::AttachWeapon::Exit()
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

int player_state::AttachWeapon::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
