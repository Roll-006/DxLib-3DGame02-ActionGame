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
	m_elapsed_time += FPS::GetDeltaTime();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::EquipKnife::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
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
	// 銃装備状態(強制的)
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
