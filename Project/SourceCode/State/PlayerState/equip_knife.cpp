#include "equip_knife.hpp"

player_state::EquipKnife::EquipKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife))
{

}

player_state::EquipKnife::~EquipKnife()
{

}

void player_state::EquipKnife::Update(Player* obj)
{
	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::EquipKnife::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::EquipKnife::Enter(Player* obj)
{

}

void player_state::EquipKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::EquipKnife::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// ナイフエイミング状態
	if (command->IsExecuting(CommandKind::kAimKnife) && obj->GetCurrentEquipKnife())
	{
		return state_controller->GetState<AimKnife, Player>();
	}
	// 切り裂く(第一段階)
	if (command->IsExecuting(CommandKind::kAttack))
	{
		return state_controller->GetState<FirstSideSlashKnife, Player>();
	}
	// 銃装備状態
	if (state_controller->TryEquipGun(obj))
	{
		return state_controller->GetState<EquipGun, Player>();
	}

	return nullptr;
}
