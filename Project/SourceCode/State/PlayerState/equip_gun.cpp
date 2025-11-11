#include "equip_gun.hpp"

player_state::EquipGun::EquipGun() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kEquipGun)),
	m_possible_aim_timer	(0.0f),
	m_is_stop_all_state		(false)
{

}

player_state::EquipGun::~EquipGun()
{

}

void player_state::EquipGun::Update(std::shared_ptr<Player>& obj)
{
	if (CommandHandler::GetInstance()->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent))
	{
		const auto time_manager = GameTimeManager::GetInstance();
		m_possible_aim_timer += time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer);
	}
	else
	{
		m_possible_aim_timer = 0.0f;
	}

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::EquipGun::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::EquipGun::Enter(std::shared_ptr<Player>& obj)
{
	m_possible_aim_timer = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

void player_state::EquipGun::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

std::shared_ptr<IState<Player>> player_state::EquipGun::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 銃エイミング状態
	if (obj->CanControl() && CommandHandler::GetInstance()->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent))
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
	if (state_controller->TryAimKnife(obj))
	{
		return state_controller->GetState<AimKnife, Player>();
	}
	// 回転切り
	if (state_controller->TrySpinningSlash(obj))
	{
		return state_controller->GetState<SpinningSlashKnife, Player>();
	}
	// 切り裂く(第一段階)
	if (state_controller->TryFirstSideSlashKnife(obj))
	{
		return state_controller->GetState<FirstSideSlashKnife, Player>();
	}

	return nullptr;
}
