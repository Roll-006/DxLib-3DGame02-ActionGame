#include "weapon_action_null.hpp"

player_state::WeaponActionNull::WeaponActionNull() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull))
{

}

player_state::WeaponActionNull::~WeaponActionNull()
{

}

void player_state::WeaponActionNull::Update(Player* obj)
{
	
}

void player_state::WeaponActionNull::LateUpdate(Player* obj)
{

}

void player_state::WeaponActionNull::Enter(Player* obj)
{
	if (obj->GetCurrentHeldWeapon())
	{
		obj->ReleaseWeapon();
	}
}

void player_state::WeaponActionNull::Exit(Player* obj)
{
	
}

std::shared_ptr<IState<Player>> player_state::WeaponActionNull::ChangeState(Player* obj)
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
	if (command->IsExecute(CommandKind::kAimKnife, TimeKind::kCurrent) && obj->GetCurrentEquipKnife())
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
