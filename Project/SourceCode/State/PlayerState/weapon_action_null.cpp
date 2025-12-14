#include "../../Object/player.hpp"
#include "../../Part/player_state_controller.hpp"
#include "weapon_action_null.hpp"

player_state::WeaponActionNull::WeaponActionNull(Player& player) :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull)),
	m_is_stop_all_state		(false),
	m_player				(player)
{

}

player_state::WeaponActionNull::~WeaponActionNull()
{

}

void player_state::WeaponActionNull::Update()
{
	
}

void player_state::WeaponActionNull::LateUpdate()
{

}

void player_state::WeaponActionNull::Enter()
{
	if (m_player.GetCurrentHeldWeapon())
	{
		m_player.ReleaseWeapon();
	}
}

void player_state::WeaponActionNull::Exit()
{
	
}

int player_state::WeaponActionNull::GetNextStateKind()
{
	if (m_player.GetDeltaTime() <= 0.0f) { return static_cast<int>(player_state::ActionStateKind::kNone); }

	const auto state_controller = m_player.GetStateController();

	// 銃装備状態
	if (state_controller->TryEquipGunShortcut())
	{
		return static_cast<int>(player_state::WeaponActionStateKind::kEquipGun);
	}
	// 銃装備状態
	else if (state_controller->TryEquipGun())
	{
		return static_cast<int>(player_state::WeaponActionStateKind::kEquipGun);
	}
	// リロード
	else if (state_controller->TryReload())
	{
		return static_cast<int>(player_state::WeaponActionStateKind::kReload);
	}
	// ナイフ装備状態
	else if (state_controller->GetActionStateKind(TimeKind::kPrev) == player_state::ActionStateKind::kStealthKill)
	{
		return static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife);
	}
	//// ナイフエイミング状態
	//if (state_controller->TryAimKnife(obj))
	//{
	//	return state_controller->GetState<AimKnife, Player>();
	//}
	// 回転切り
	else if (state_controller->TrySpinningSlash())
	{
		return static_cast<int>(player_state::WeaponActionStateKind::kSpinningSlashKnife);
	}
	// 切り裂く(第一段階)
	else if (state_controller->TryFirstSideSlashKnife())
	{
		return static_cast<int>(player_state::WeaponActionStateKind::kFirstSideSlashKnife);
	}

	return static_cast<int>(player_state::ActionStateKind::kNone);
}
