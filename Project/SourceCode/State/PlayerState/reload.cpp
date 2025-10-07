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

	const auto gun = std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	if (obj->GetAnimator()->GetPlayRate(AnimatorBase::BodyKind::kUpperBody) > 0.5f)
	{
		if (!m_is_reloaded)
		{
			gun->GetMagazine()->OnReloaded();

			obj->SetRemainingBulletNum(gun->OnReload(obj->GetCurrentRemainingBulletNum()));
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
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	
	// eƒGƒCƒ~ƒ“ƒOó‘Ô
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody)
		&& command->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent))
	{
		return state_controller->GetState<AimGun, Player>();
	}
	//// e‘•”õó‘Ô
	//if (state_controller->TryEquipGunShortcut(obj))
	//{
	//	return state_controller->GetState<EquipGun, Player>();
	//}
	// e‘•”õó‘Ô
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<EquipGun, Player>();
	}

	return nullptr;
}
