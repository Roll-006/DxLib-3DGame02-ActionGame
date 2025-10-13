#include "spinning_slash_knife.hpp"

player_state::SpinningSlashKnife::SpinningSlashKnife() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kSpinningSlashKnife)),
	m_combo_timer			(0.0f),
	m_is_stop_all_state		(false)
{

}

player_state::SpinningSlashKnife::~SpinningSlashKnife()
{

}

void player_state::SpinningSlashKnife::Update(std::shared_ptr<Player>& obj)
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_combo_timer += time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);

	//obj->CalcMoveOffsetSideSlashKnife();

	obj->AllowCalcLookDir();
	obj->GetCurrentHeldWeapon()->Update();
	obj->SpinningSlashKnifeOffsetMove();
}

void player_state::SpinningSlashKnife::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::SpinningSlashKnife::Enter(std::shared_ptr<Player>& obj)
{
	m_combo_timer = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::SpinningSlashKnife::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::SpinningSlashKnife::ChangeState(std::shared_ptr<Player>& obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 切り裂く(第一段階)
	if (m_combo_timer > kComboValidTime && command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent))
	{
		return state_controller->GetState<SecondSideSlashKnife, Player>();
	}
	// ナイフ装備状態
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<EquipKnife, Player>();
	}

	return nullptr;
}
