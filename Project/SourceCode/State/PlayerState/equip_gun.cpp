#include "equip_gun.hpp"

player_state::EquipGun::EquipGun() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kEquipGun)),
	m_possible_aim_timer	(0.0f)
{

}

player_state::EquipGun::~EquipGun()
{

}

void player_state::EquipGun::Update(Player* obj)
{
	if (CommandHandler::GetInstance()->IsExecuting(CommandKind::kAimGun))
	{
		const auto time_manager = GameTimeManager::GetInstance();
		m_possible_aim_timer += time_manager->GetDeltaTime(TimeScaleController::LayerKind::kPlayer);
	}
	else
	{
		m_possible_aim_timer = 0.0f;
	}

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::EquipGun::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::EquipGun::Enter(Player* obj)
{
	m_possible_aim_timer = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipWeapon());
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::EquipGun::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon());
}

std::shared_ptr<IState<Player>> player_state::EquipGun::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 銃エイミング状態
	if (CommandHandler::GetInstance()->IsExecuting(CommandKind::kAimGun))
	{	
		if (m_possible_aim_timer >= kPossibleAimTime)
		{
			return state_controller->GetState<AimGun, Player>();
		}
	}
	// リロード
	if (state_controller->TryReload(obj))
	{
		return state_controller->GetState<Reload, Player>();
	}
	// ナイフエイミング状態
	if (command->IsExecuting(CommandKind::kAimKnife) && obj->GetCurrentEquipKnife())
	{
		return state_controller->GetState<AimKnife, Player>();
	}
	// 回転切り
	if (state_controller->TrySpinningSlash())
	{
		return state_controller->GetState<SpinningSlashKnife, Player>();
	}
	// 切り裂く(第一段階)
	if (command->IsExecuting(CommandKind::kAttack))
	{
		return state_controller->GetState<FirstSideSlashKnife, Player>();
	}

	return nullptr;
}
