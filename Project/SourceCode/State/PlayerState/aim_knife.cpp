#include "aim_knife.hpp"

player_state::AimKnife::AimKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAimKnife))
{

}

player_state::AimKnife::~AimKnife()
{

}

void player_state::AimKnife::Update(Player* obj)
{
	obj->SetLookDirCorrectValueForAim();
	obj->DirOfCameraForward();
}

void player_state::AimKnife::LateUpdate(Player* obj)
{

}

void player_state::AimKnife::Enter(Player* obj)
{

}

void player_state::AimKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::AimKnife::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// ƒiƒCƒt‘•”õó‘Ô
	if (!command->IsExecuting(CommandKind::kAimKnife))
	{
		return state_controller->GetState<EquipKnife, Player>();
	}
	// “Ë‚«Žh‚·
	if (command->IsExecuting(CommandKind::kAttack))
	{
		return state_controller->GetState<StabKnife, Player>();
	}

	return nullptr;
}
