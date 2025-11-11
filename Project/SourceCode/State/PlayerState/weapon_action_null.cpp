#include "weapon_action_null.hpp"

player_state::WeaponActionNull::WeaponActionNull() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull)),
	m_is_stop_all_state		(false)
{

}

player_state::WeaponActionNull::~WeaponActionNull()
{

}

void player_state::WeaponActionNull::Update(std::shared_ptr<Player>& obj)
{
	
}

void player_state::WeaponActionNull::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::WeaponActionNull::Enter(std::shared_ptr<Player>& obj)
{
	if (obj->GetCurrentHeldWeapon())
	{
		obj->ReleaseWeapon();
	}
}

void player_state::WeaponActionNull::Exit(std::shared_ptr<Player>& obj)
{
	
}

std::shared_ptr<IState<Player>> player_state::WeaponActionNull::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

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
	// リロード
	if (state_controller->TryReload(obj))
	{
		return state_controller->GetState<Reload, Player>();
	}
	// ナイフ装備状態
	if (static_cast<player_state::ActionStateKind>(state_controller->GetActionState(TimeKind::kPrev)->GetStateKind()) == player_state::ActionStateKind::kStealthKill)
	{
		return state_controller->GetState<EquipKnife, Player>();
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
