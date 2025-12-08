#include "weapon_shortcut_selecter.hpp"

#include "../Object/player.hpp"
#include "../Part/player_state_controller.hpp"
#include "../Command/command_handler.hpp"

WeaponShortcutSelecter::WeaponShortcutSelecter() : 
	m_current_select_shortcut	(WeaponShortcutPosKind::kInsideUp),
	m_is_selecting				(false)
{
	
}

WeaponShortcutSelecter::~WeaponShortcutSelecter()
{

}

void WeaponShortcutSelecter::Update(const std::shared_ptr<Player>& player)
{
	if (!player->CanControl())			{ return; }
	if (player->GetDeltaTime() == 0.0f) { return; }

	const auto state_controller		= player->GetStateController();
	const auto weapon_action_state	= state_controller->GetWeaponActionState(TimeKind::kCurrent);
	const auto action_state			= state_controller->GetActionState(TimeKind::kCurrent);

	if (   weapon_action_state	->GetStateKind() == static_cast<int>(player_state::WeaponActionStateKind::kShotRocketLauncher)
		|| action_state			->GetStateKind() == static_cast<int>(player_state::ActionStateKind		::kDead))
	{
		return;
	}

	m_is_selecting = false;

	SelectWeaponByKey();
	SelectWeaponByPad();

	HoldWeapon(player);
}

void WeaponShortcutSelecter::AttachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind, const std::shared_ptr<WeaponBase>& weapon)
{
	m_shortcut_weapons[shortcut_pos_kind] = weapon;
}

void WeaponShortcutSelecter::DetachShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	m_shortcut_weapons[shortcut_pos_kind] = nullptr;
}

std::shared_ptr<WeaponBase> WeaponShortcutSelecter::GetShortcutWeapon(const WeaponShortcutPosKind shortcut_pos_kind)
{
	return m_shortcut_weapons.contains(shortcut_pos_kind) ? m_shortcut_weapons[shortcut_pos_kind] : nullptr;
}

void WeaponShortcutSelecter::SelectWeaponByPad()
{
	for (int i = 0; i < 4; ++i)
	{
		const auto command		= CommandHandler::GetInstance();
		const auto command_num	= static_cast<int>(CommandKind::kSelectWeaponUp) + i;
		
		if (command->IsExecute(static_cast<CommandKind>(command_num), TimeKind::kCurrent))
		{
			auto	   current_shortcut_num = static_cast<int>(m_current_select_shortcut);
			const auto is_inside			= current_shortcut_num < 4;
			const auto increase_value		= is_inside ? 4 : -4;
			const auto offset				= is_inside ? 0 :  4;
			const auto is_select_same_dir	= current_shortcut_num == i + offset ? true : false;

			// 既に入力した方向にいた場合は内側 / 外側の切り替えを行う
			if (is_select_same_dir)
			{
				current_shortcut_num += increase_value;
			}
			else
			{
				current_shortcut_num = i;
			}

			m_current_select_shortcut	= static_cast<WeaponShortcutPosKind>(current_shortcut_num);
			m_is_selecting				= true;
			break;
		}
	}
}

void WeaponShortcutSelecter::SelectWeaponByKey()
{
	const auto command = CommandHandler::GetInstance();

	// 直接選択
	for (int i = 0; i < 8; ++i)
	{
		const auto command_num  = static_cast<int>(CommandKind::kSelectWeaponInsideUp) + i;

		if (command->IsExecute(static_cast<CommandKind>(command_num), TimeKind::kCurrent))
		{
			const auto shortcut_num = static_cast<int>(WeaponShortcutPosKind::kInsideUp) + i;

			m_current_select_shortcut	= static_cast<WeaponShortcutPosKind>(shortcut_num);
			m_is_selecting				= true;
			break;
		}
	}

	// 内側 / 外側の移動
	if (command->IsExecute(CommandKind::kSideChangeWeapon, TimeKind::kCurrent))
	{
		const auto current_shortcut_num	= static_cast<int>(m_current_select_shortcut);
		const auto increase_value		= current_shortcut_num < 4 ? 4 : -4;
		const auto result_shortcut		= current_shortcut_num + increase_value;

		m_current_select_shortcut	= static_cast<WeaponShortcutPosKind>(result_shortcut);
		m_is_selecting				= true;
	}

	// 回転選択
	SelectWeaponRotate(CommandKind::kSelectWeaponRotateRight);
	SelectWeaponRotate(CommandKind::kSelectWeaponRotateLeft);
}

void WeaponShortcutSelecter::SelectWeaponRotate(const CommandKind command_kind)
{
	if (command_kind != CommandKind::kSelectWeaponRotateRight && command_kind != CommandKind::kSelectWeaponRotateLeft) { return; }

	const auto command = CommandHandler::GetInstance();

	// 内側 / 外側切り替えの入力モードを強制的にホールド式へ変更
	const auto input_mode_kind = command->GetInputModeKind(CommandKind::kSideChangeWeapon);
	command->SetInputMode(CommandKind::kSideChangeWeapon, InputModeKind::kHold);

	if (command->IsExecute(command_kind, TimeKind::kCurrent))
	{
		auto	   current_shortcut_num		= static_cast<int>(m_current_select_shortcut);
		const auto increase_value			= command_kind == CommandKind::kSelectWeaponRotateLeft ? -1 : 1;
		const auto is_inside				= current_shortcut_num < 4;
		const auto is_select_inside			= !command->IsExecute(CommandKind::kSideChangeWeapon, TimeKind::kCurrent);
		const auto max_select_num			= is_select_inside ? 3 : 7;
		const auto min_select_num			= is_select_inside ? 0 : 4;

		if (is_select_inside)
		{
			// 外側であった場合、強制的に内側に移動させる
			if (!is_inside) { current_shortcut_num -= 4; }
		}
		else
		{
			// 内側であった場合、強制的に外側に移動させる
			if (is_inside)  { current_shortcut_num += 4; }
		}

		// 加算およびループ処理
		current_shortcut_num += increase_value;
		if (current_shortcut_num > max_select_num) { current_shortcut_num = min_select_num; }
		if (current_shortcut_num < min_select_num) { current_shortcut_num = max_select_num; }

		m_current_select_shortcut	= static_cast<WeaponShortcutPosKind>(current_shortcut_num);
		m_is_selecting				= true;
	}

	// 内側 / 外側切り替えの入力モードを元の状態へ戻す
	command->SetInputMode(CommandKind::kSideChangeWeapon, input_mode_kind);
}

void WeaponShortcutSelecter::HoldWeapon(std::shared_ptr<Player> player)
{
	const auto select_weapon = GetShortcutWeapon(m_current_select_shortcut);

	if (select_weapon && m_is_selecting)
	{
		// 現在手に持っている武器を装着する
		const auto current_held_weapon = player->GetCurrentHeldWeapon();
		if (current_held_weapon && current_held_weapon != select_weapon)
		{
			player->AttachWeapon(current_held_weapon);
		}

		player->DetachWeapon(select_weapon);
		player->EquipWeapon	(select_weapon, WeaponSlotKind::kMain);
		player->HoldWeapon	(select_weapon);
	}
}
