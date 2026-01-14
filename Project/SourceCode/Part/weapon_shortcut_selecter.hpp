#pragma once
#include "../Base/weapon_base.hpp"
#include "../Kind/weapon_shortcut_pos_kind.hpp"
#include "../Kind/command_kind.hpp"

class Player;

class WeaponShortcutSelecter final
{
public:
	WeaponShortcutSelecter();
	~WeaponShortcutSelecter();

	void Update(const std::shared_ptr<Player>& player);

	void AttachShortcutWeapon(const WeaponShortcutPosKind pos_kind, const std::shared_ptr<WeaponBase>& weapon);
	void DetachShortcutWeapon(const WeaponShortcutPosKind pos_kind);

	[[nodiscard]] std::shared_ptr<WeaponBase>	GetShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind) const;
	[[nodiscard]] WeaponShortcutPosKind			GetCurrentSelectShortcut()	const { return m_current_select_shortcut; }
	[[nodiscard]] bool							isSelecting()				const { return m_is_selecting; }
	
private:
	void SelectWeaponByPad();
	void SelectWeaponByKey();
	void SelectWeaponRotate(const CommandKind command_kind);

	/// @brief 武器を持たせる
	void HoldWeapon(std::shared_ptr<Player> player);

private:
	std::unordered_map<WeaponShortcutPosKind, std::shared_ptr<WeaponBase>>	m_shortcut_weapons;			// ショートカットに登録している武器
	WeaponShortcutPosKind													m_current_select_shortcut;	// 現在選択しているショートカット
	bool																	m_is_selecting;
};
