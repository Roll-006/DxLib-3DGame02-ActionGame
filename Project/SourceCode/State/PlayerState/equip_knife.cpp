#include "equip_knife.hpp"

player_state::EquipKnife::EquipKnife() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife)),
	m_elapsed_time			(0.0f)
{

}

player_state::EquipKnife::~EquipKnife()
{

}

void player_state::EquipKnife::Update(Player* obj)
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_elapsed_time += time_manager->GetDeltaTime(TimeScaleController::LayerKind::kPlayer);

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::EquipKnife::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::EquipKnife::Enter(Player* obj)
{
	m_elapsed_time = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::EquipKnife::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::EquipKnife::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

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
	// e‘•”õó‘Ô(‹­§“I)
	if (m_elapsed_time > kReleaseKinfeForciblyTime && obj->GetCurrentEquipWeaponKind() == WeaponKind::kGun)
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// NULL
	if (m_elapsed_time > kReleaseKinfeForciblyTime)
	{
		return state_controller->GetState<WeaponActionNull, Player>();
	}

	return nullptr;
}
