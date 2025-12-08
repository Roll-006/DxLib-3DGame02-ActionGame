#include "reload.hpp"

player_state::Reload::Reload() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kReload)),
	m_is_stop_all_state		(false),
	m_is_release_ammo_box	(false),
	m_is_set_ammo_box		(false),
	m_is_cocking			(false)
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
	const auto play_rate	= animator->GetPlayRate(AnimatorBase::BodyKind::kUpperBody);
	const auto event_system = EventSystem::GetInstance();

	// 弾倉を外す判定
	if (play_rate > 0.0f && !m_is_release_ammo_box)
	{
		event_system->Publish(ReleaseAmmoBoxEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
		m_is_release_ammo_box = true;
	}

	// 弾倉をセット判定
	if (play_rate > 0.5f && !m_is_set_ammo_box)
	{
		event_system->Publish(SetAmmoBoxEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));

		gun->GetMagazine()->OnReloaded();
		obj->GetAmmoHolder()->Reload(gun);
		m_is_set_ammo_box = true;
	}

	// コッキング判定
	if (play_rate > 0.7f && !m_is_cocking)
	{
		event_system->Publish(CockingEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
		m_is_cocking = true;
	}
}

void player_state::Reload::LateUpdate(std::shared_ptr<Player>& obj)
{
	
}

void player_state::Reload::Enter(std::shared_ptr<Player>& obj)
{
	m_is_release_ammo_box	= false;
	m_is_set_ammo_box		= false;
	m_is_cocking			= false;

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
