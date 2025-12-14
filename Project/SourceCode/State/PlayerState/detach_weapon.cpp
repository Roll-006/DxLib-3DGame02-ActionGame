#include "detach_weapon.hpp"

player_state::DetachWeapon::DetachWeapon(Player& player) :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kStabKnife)),
	m_is_stop_all_state		(false),
	m_player				(player)
{

}

player_state::DetachWeapon::~DetachWeapon()
{

}

void player_state::DetachWeapon::Update()
{
	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::DetachWeapon::LateUpdate()
{
	
}

void player_state::DetachWeapon::Enter()
{
	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::DetachWeapon::Exit()
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

int player_state::DetachWeapon::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
