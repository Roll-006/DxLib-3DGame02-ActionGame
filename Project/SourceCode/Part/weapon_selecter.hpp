#pragma once
#include "../Base/one_instance_singleton_base.hpp"

#include "../Base/weapon_base.hpp"
#include "../Data/Kind/weapon_shortcut_pos_kind.hpp"
#include "../Data/Kind/command_kind.hpp"

class WeaponSelecter final : public OneInstanceSingletonBase<WeaponSelecter>
{
public:
	WeaponSelecter(const std::shared_ptr<WeaponBase> equip_weapon);
	~WeaponSelecter();

	void Update();

	void AttachShortcutWeapon(const WeaponShortcutPosKind pos_kind, const std::shared_ptr<WeaponBase> weapon);
	void DetachShortcutWeapon(const WeaponShortcutPosKind pos_kind);

	[[nodiscard]] std::shared_ptr<WeaponBase>	GetShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind);
	[[nodiscard]] WeaponShortcutPosKind			GetCurrentSelectShortcut() const { return m_current_select_shortcut; }
	
private:
	void SelectWeaponByPad();
	void SelectWeaponByKey();
	void SelectWeaponRotate(const CommandKind command_kind);

private:
	std::unordered_map<WeaponShortcutPosKind, std::shared_ptr<WeaponBase>>	m_shortcut_weapons;			// ショートカットに登録している武器
	std::shared_ptr<WeaponBase>												m_current_equip_weapon;		// 現在装備している武器
	WeaponShortcutPosKind													m_current_select_shortcut;	// 現在選択しているショートカット

	friend OneInstanceSingletonBase<WeaponSelecter>;
};
