#include "weapon_action_null.hpp"

player_state::WeaponActionNull::WeaponActionNull() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull)),
	m_is_all_stop			(false)
{

}

player_state::WeaponActionNull::~WeaponActionNull()
{

}

void player_state::WeaponActionNull::Update(std::shared_ptr<Player>& obj)
{
	
}

void player_state::WeaponActionNull::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::WeaponActionNull::Enter(std::shared_ptr<Player>& obj)
{
	if (obj->GetCurrentHeldWeapon())
	{
		obj->ReleaseWeapon();
	}
}

void player_state::WeaponActionNull::Exit(std::shared_ptr<Player>& obj)
{
	
}

std::shared_ptr<IState<Player>> player_state::WeaponActionNull::ChangeState(std::shared_ptr<Player>& obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// e‘•”õó‘Ô
	if (state_controller->TryEquipGunShortcut(obj))
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// e‘•”õó‘Ô
	if (state_controller->TryEquipGun(obj))
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// ƒiƒCƒtƒGƒCƒ~ƒ“ƒOó‘Ô
	if (command->IsExecute(CommandKind::kAimKnife, TimeKind::kCurrent) && obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub))
	{
		return state_controller->GetState<AimKnife, Player>();
	}
	// ‰ñ“]Ø‚è
	if (state_controller->TrySpinningSlash())
	{
		return state_controller->GetState<SpinningSlashKnife, Player>();
	}
	// Ø‚è—ô‚­(‘æˆê’iŠK)
	if (command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent))
	{
		return state_controller->GetState<FirstSideSlashKnife, Player>();
	}

	return nullptr;
}
