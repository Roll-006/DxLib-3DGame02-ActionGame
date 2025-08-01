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
	
}

void player_state::WeaponActionNull::Exit(Player* obj)
{
	
}

std::shared_ptr<IState<Player>> player_state::WeaponActionNull::ChangeState(const Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 銃装備状態
	if (command->IsExecuting(CommandKind::kAimGun))
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// ナイフエイミング状態
	if (command->IsExecuting(CommandKind::kAimKnife))
	{
		return state_controller->GetState<AimKnife, Player>();
	}
	// 切り裂く(第一段階)
	if (command->IsExecuting(CommandKind::kAttack))
	{
		return state_controller->GetState<FirstSideSlashKnife, Player>();
	}

	return nullptr;
}
