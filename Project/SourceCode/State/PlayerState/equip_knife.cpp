#include "equip_knife.hpp"

player_state::EquipKnife::EquipKnife() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife)),
	m_elapsed_time			(0.0f),
	m_is_stop_all_state		(false)
{

}

player_state::EquipKnife::~EquipKnife()
{

}

void player_state::EquipKnife::Update(std::shared_ptr<Player>& obj)
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_elapsed_time += time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer);

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::EquipKnife::LateUpdate(std::shared_ptr<Player>& obj)
{
	
}

void player_state::EquipKnife::Enter(std::shared_ptr<Player>& obj)
{
	m_elapsed_time = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon	 (obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::EquipKnife::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon (obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::EquipKnife::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

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
	// 銃装備状態
	if (state_controller->TryEquipGunShortcut(obj))
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// 銃装備状態
	if (state_controller->TryEquipGun(obj))
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// 銃装備状態(強制的)
	if (m_elapsed_time > kReleaseKinfeForciblyTime && obj->GetCurrentEquipWeaponKind(WeaponSlotKind::kMain) == WeaponKind::kGun)
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// リロード
	if (state_controller->TryReload(obj))
	{
		return state_controller->GetState<Reload, Player>();
	}
	// NULL
	if (m_elapsed_time > kReleaseKinfeForciblyTime)
	{
		return state_controller->GetState<WeaponActionNull, Player>();
	}

	return nullptr;
}
