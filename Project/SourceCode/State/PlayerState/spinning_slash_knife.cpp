#include "spinning_slash_knife.hpp"

player_state::SpinningSlashKnife::SpinningSlashKnife() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kSpinningSlashKnife)),
	m_combo_timer			(0.0f)
{

}

player_state::SpinningSlashKnife::~SpinningSlashKnife()
{

}

void player_state::SpinningSlashKnife::Update(Player* obj)
{
	m_combo_timer += FPS::GetDeltaTime();

	//obj->CalcMoveOffsetSideSlashKnife();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::SpinningSlashKnife::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::SpinningSlashKnife::Enter(Player* obj)
{
	m_combo_timer = 0.0f;

	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::SpinningSlashKnife::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::SpinningSlashKnife::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 切り裂く(第一段階)
	if (m_combo_timer > kComboValidTime && command->IsExecuting(CommandKind::kAttack))
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
