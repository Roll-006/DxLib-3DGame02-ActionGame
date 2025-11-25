#include "reload.hpp"

player_state::Reload::Reload() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kReload)),
	m_is_reloaded			(false),
	m_is_stop_all_state		(false)
{

}

player_state::Reload::~Reload()
{

}

void player_state::Reload::Update(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->Update();

	// アニメーションが一定まで進むまでリロードを許可しない
	const auto animator		= obj->GetAnimator();
	const auto anim_kind	= static_cast<PlayerAnimKind>(animator->GetAnimKind(AnimatorBase::BodyKind::kUpperBody, TimeKind::kCurrent));
	const auto gun			= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	if (animator->GetPlayRate(AnimatorBase::BodyKind::kUpperBody) > 0.5f && anim_kind == PlayerAnimKind::kReload)
	{
		if (!m_is_reloaded)
		{
			gun->GetMagazine()->OnReloaded();
			obj->GetAmmoHolder()->Reload(gun);
			m_is_reloaded = true;
		}
	}
}

void player_state::Reload::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::Reload::Enter(std::shared_ptr<Player>& obj)
{
	m_is_reloaded = false;

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));

	const auto gun = std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	gun->GetMagazine()->OnStartReload(obj->GetModeler());
}

void player_state::Reload::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

std::shared_ptr<IState<Player>> player_state::Reload::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	
	// 銃エイミング状態
	if (obj->CanControl()
		&& obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody)
		&& command->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent))
	{
		return state_controller->GetState<AimGun, Player>();
	}
	//// 銃装備状態
	//if (state_controller->TryEquipGunShortcut(obj))
	//{
	//	return state_controller->GetState<EquipGun, Player>();
	//}
	// 銃装備状態
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<EquipGun, Player>();
	}

	return nullptr;
}
