#include "weapon_selecter.hpp"
#include "../Command/command_handler.hpp"

WeaponSelecter::WeaponSelecter(const std::shared_ptr<WeaponBase> equip_weapon) : 
	m_current_equip_weapon		(equip_weapon),
	m_current_select_shortcut	(WeaponShortcutPosKind::kInsideUp)
{
	
}

WeaponSelecter::~WeaponSelecter()
{

}

void WeaponSelecter::Update()
{
	SelectWeaponByKey();
	SelectWeaponByPad();
}

void WeaponSelecter::AttachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind, const std::shared_ptr<WeaponBase> weapon)
{
	m_shortcut_weapons[shortcut_pos_kind] = weapon;
}

void WeaponSelecter::DetachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	m_shortcut_weapons[shortcut_pos_kind] = nullptr;
}

std::shared_ptr<WeaponBase> WeaponSelecter::GetShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	return m_shortcut_weapons.count(shortcut_pos_kind) ? m_shortcut_weapons[shortcut_pos_kind] : nullptr;
}

void WeaponSelecter::SelectWeaponByPad()
{

}

void WeaponSelecter::SelectWeaponByKey()
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

void WeaponSelecter::SelectWeaponRotate(const CommandKind command_kind)
{
	if (command_kind != CommandKind::kSelectWeaponRotateRight && command_kind != CommandKind::kSelectWeaponRotateLeft) { return; }

	if (CommandHandler::GetInstance()->IsExecuting(command_kind))
	{
		const bool	is_left_rotation		= command_kind == CommandKind::kSelectWeaponRotateLeft ? true : false;
		int			current_shortcut_num	= static_cast<int>(m_current_select_shortcut);
		const bool	is_outside				= current_shortcut_num >= 4;
		const int	increase_value			= is_left_rotation ? 1 : -1;

		// 外側であった場合、強制的に内側に移動させる
		if (is_outside)
		{
			current_shortcut_num -= 4;
		}

		current_shortcut_num += increase_value;

		if (is_left_rotation)
		{
			if (current_shortcut_num > 3)
			{
				current_shortcut_num = 0;
			}
		}
		else
		{
			if (current_shortcut_num < 0)
			{
				current_shortcut_num = 3;
			}
		}

		m_current_select_shortcut = static_cast<WeaponShortcutPosKind>(current_shortcut_num);
	}
}
