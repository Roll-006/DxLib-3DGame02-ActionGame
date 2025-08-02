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

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AimGun::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::AimGun::Enter(Player* obj)
{
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::AimGun::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::AimGun::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();

	if (!CommandHandler::GetInstance()->IsExecuting(CommandKind::kAimGun))
	{
		return state_controller->GetState<EquipGun, Player>();
	}

	return nullptr;
}
