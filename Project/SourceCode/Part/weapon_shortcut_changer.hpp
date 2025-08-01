#pragma once
#include "../Base/one_instance_singleton_base.hpp"

#include "../Base/weapon_base.hpp"
#include "../Data/Kind/weapon_shortcut_pos_kind.hpp"

class WeaponShortcutChanger final : public OneInstanceSingletonBase<WeaponShortcutChanger>
{
public:
	WeaponShortcutChanger(const std::shared_ptr<WeaponBase> equip_weapon);
	~WeaponShortcutChanger();

	void AttachShortcutWeapon(const WeaponShortcutPosKind pos_kind, const std::shared_ptr<WeaponBase> weapon);
	void DetachShortcutWeapon(const WeaponShortcutPosKind pos_kind);

	[[nodiscard]] std::shared_ptr<WeaponBase> GetShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind);
	
private:
	std::unordered_map<WeaponShortcutPosKind, std::shared_ptr<WeaponBase>> m_shortcut_weapons;
	std::shared_ptr<WeaponBase> m_current_equip_weapon;

	friend OneInstanceSingletonBase<WeaponShortcutChanger>;
};
