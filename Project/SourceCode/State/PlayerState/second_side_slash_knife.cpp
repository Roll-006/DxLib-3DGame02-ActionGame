#include "second_side_slash_knife.hpp"

player_state::SecondSideSlashKnife::SecondSideSlashKnife() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kSecondSideSlashKnife)),
	m_combo_timer			(0.0f)
{

}

player_state::SecondSideSlashKnife::~SecondSideSlashKnife()
{

}

void player_state::SecondSideSlashKnife::Update(Player* obj)
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_combo_timer += time_manager->GetDeltaTime(TimeScaleController::LayerKind::kWorld);

	//obj->CalcMoveOffsetSideSlashKnife();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::SecondSideSlashKnife::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::SecondSideSlashKnife::Enter(Player* obj)
{
	m_combo_timer = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::SecondSideSlashKnife::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::SecondSideSlashKnife::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command = CommandHandler::GetInstance();

	// 切り裂く(第一段階)
	if (m_combo_timer > kComboValidTime && command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent))
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
