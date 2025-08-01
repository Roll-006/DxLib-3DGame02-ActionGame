#include "weapon_shortcut_changer.hpp"

WeaponShortcutChanger::WeaponShortcutChanger(const std::shared_ptr<WeaponBase> equip_weapon) : 
	m_current_equip_weapon(equip_weapon)
{

}

WeaponShortcutChanger::~WeaponShortcutChanger()
{

}

void WeaponShortcutChanger::AttachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind, const std::shared_ptr<WeaponBase> weapon)
{
	m_shortcut_weapons[shortcut_pos_kind] = weapon;
}

void WeaponShortcutChanger::DetachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	m_shortcut_weapons[shortcut_pos_kind] = nullptr;
}

std::shared_ptr<WeaponBase> WeaponShortcutChanger::GetShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	return m_shortcut_weapons.count(shortcut_pos_kind) ? m_shortcut_weapons[shortcut_pos_kind] : nullptr;
}
