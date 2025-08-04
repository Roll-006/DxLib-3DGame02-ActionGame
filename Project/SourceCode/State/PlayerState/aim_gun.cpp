#include "aim_gun.hpp"

player_state::AimGun::AimGun() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAimGun))
{
	m_stop_states.emplace_back();
}

player_state::AimGun::~AimGun()
{

}

void player_state::AimGun::Update(Player* obj)
{
	obj->SetLookDirOffsetValueForAim();
	obj->DirOfCameraForward();

	// ˆø‚«‹à‚ð
	if (CommandHandler::GetInstance()->GetInstance()->IsExecuting(CommandKind::kAttack))
	{

	}

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AimGun::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::AimGun::Enter(Player* obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipWeapon());
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::AimGun::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon());
}

std::shared_ptr<IState<Player>> player_state::AimGun::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// e‘•”õó‘Ô
	if (!command->IsExecuting(CommandKind::kAimGun))
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// ƒVƒ‡ƒbƒg
	if (command->IsExecuting(CommandKind::kAttack))
	{
		return state_controller->GetState<Shot, Player>();
	}

	return nullptr;
}
