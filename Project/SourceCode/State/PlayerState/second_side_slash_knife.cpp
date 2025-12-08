#include "second_side_slash_knife.hpp"

player_state::SecondSideSlashKnife::SecondSideSlashKnife() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kSecondSideSlashKnife)),
	m_combo_timer			(0.0f),
	m_is_stop_all_state		(false)
{

}

player_state::SecondSideSlashKnife::~SecondSideSlashKnife()
{

}

void player_state::SecondSideSlashKnife::Update(std::shared_ptr<Player>& obj)
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_combo_timer += time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);

	//obj->CalcMoveOffsetSideSlashKnife();

	obj->AllowCalcLookDir();
	obj->GetCurrentHeldWeapon()->Update();
	obj->SideSlashKnifeOffsetMove();
}

void player_state::SecondSideSlashKnife::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::SecondSideSlashKnife::Enter(std::shared_ptr<Player>& obj)
{
	m_combo_timer = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::SecondSideSlashKnife::Exit(std::shared_ptr<Player>& obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::SecondSideSlashKnife::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command = CommandHandler::GetInstance();

	// 切り裂く(第一段階)
	if (m_combo_timer > kComboValidTime && state_controller->TryFirstSideSlashKnife(obj))
	{
		return state_controller->GetState<FirstSideSlashKnife, Player>();
	}
	// ナイフ装備状態
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<EquipKnife, Player>();
	}

	return nullptr;
}
