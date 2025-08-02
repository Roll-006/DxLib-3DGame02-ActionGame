#include "weapon_shortcut_selecter.hpp"

#include "../Object/player.hpp"
#include "../Command/command_handler.hpp"

WeaponShortcutSelecter::WeaponShortcutSelecter() : 
	m_current_select_shortcut(WeaponShortcutPosKind::kInsideUp)
{
	
}

WeaponShortcutSelecter::~WeaponShortcutSelecter()
{

}

void WeaponShortcutSelecter::Update(const Player* player)
{
	SelectWeaponByKey();
	SelectWeaponByPad();

	HoldWeapon(player);
}

void WeaponShortcutSelecter::AttachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind, const std::shared_ptr<WeaponBase> weapon)
{
	m_shortcut_weapons[shortcut_pos_kind] = weapon;
}

void WeaponShortcutSelecter::DetachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	m_shortcut_weapons[shortcut_pos_kind] = nullptr;
}

std::shared_ptr<WeaponBase> WeaponShortcutSelecter::GetShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	return m_shortcut_weapons.count(shortcut_pos_kind) ? m_shortcut_weapons[shortcut_pos_kind] : nullptr;
}

void WeaponShortcutSelecter::SelectWeaponByPad()
{

}

void WeaponShortcutSelecter::SelectWeaponByKey()
{
	const auto command = CommandHandler::GetInstance();

	// 直接選択
	for (int i = 0; i < 8; ++i)
	{
		const int command_num = static_cast<int>(CommandKind::kSelectWeaponInsideLeft) + i;
		const int shortcut_num = static_cast<int>(WeaponShortcutPosKind::kInsideLeft) + i;

		if (command->IsExecuting(static_cast<CommandKind>(command_num)))
		{
			m_current_select_shortcut = static_cast<WeaponShortcutPosKind>(shortcut_num);
			break;
		}
	}

	// 内側 / 外側の移動
	if (command->IsExecuting(CommandKind::kSideChangeWeapon))
	{
		const int current_shortcut_num = static_cast<int>(m_current_select_shortcut);
		const int decrease_value = current_shortcut_num < 4 ? 4 : -4;
		const int result_shortcut = current_shortcut_num + decrease_value;

		m_current_select_shortcut = static_cast<WeaponShortcutPosKind>(result_shortcut);
	}

	// 回転選択
	SelectWeaponRotate(CommandKind::kSelectWeaponRotateRight);
	SelectWeaponRotate(CommandKind::kSelectWeaponRotateLeft);
}

void WeaponShortcutSelecter::SelectWeaponRotate(const CommandKind command_kind)
{
	if (command_kind != CommandKind::kSelectWeaponRotateRight && command_kind != CommandKind::kSelectWeaponRotateLeft) { return; }

	if (CommandHandler::GetInstance()->IsExecuting(command_kind))
	{
		int		  current_shortcut_num	= static_cast<int>(m_current_select_shortcut);
		const int increase_value		= command_kind == CommandKind::kSelectWeaponRotateLeft ? 1 : -1;

		// 外側であった場合、強制的に内側に移動させる
		if (current_shortcut_num >= 4)
		{
			current_shortcut_num -= 4;
		}

		current_shortcut_num += increase_value;
		if (current_shortcut_num > 3) { current_shortcut_num = 0; }
		if (current_shortcut_num < 0) { current_shortcut_num = 3; }

		m_current_select_shortcut = static_cast<WeaponShortcutPosKind>(current_shortcut_num);
	}
}

void WeaponShortcutSelecter::HoldWeapon(const Player* player)
{
	if (m_shortcut_weapons[m_current_select_shortcut])
	{
		//player->EquipWeapon();
		//player->HeldWeapon();
	}
}
