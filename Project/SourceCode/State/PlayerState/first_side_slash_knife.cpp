#include "first_side_slash_knife.hpp"

player_state::FirstSideSlashKnife::FirstSideSlashKnife(Player& player) :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kFirstSideSlashKnife)),
	m_combo_timer			(0.0f),
	m_is_stop_all_state		(false),
	m_has_trigger_created	(false),
	m_has_trigger_deleted	(false),
	m_player				(player)
{

}

player_state::FirstSideSlashKnife::~FirstSideSlashKnife()
{

}

void player_state::FirstSideSlashKnife::Update()
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_combo_timer += time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);

	const auto animator		= obj->GetAnimator();
	const auto anim_kind	= static_cast<PlayerAnimKind>(animator->GetAnimKind(AnimatorBase::BodyKind::kUpperBody, TimeKind::kCurrent));
	const auto play_rate	= animator->GetPlayRate(AnimatorBase::BodyKind::kUpperBody);

	//obj->CalcMoveOffsetSideSlashKnife();

	obj->AllowCalcLookDir();
	obj->GetCurrentHeldWeapon()->Update();
	obj->SideSlashKnifeOffsetMove();

	// 攻撃判定用トリガーを生成
	if (!m_has_trigger_created)
	{
		if (play_rate > 0.3f && anim_kind == PlayerAnimKind::kFirstSideSlashKnife)
		{
			const auto knife = std::dynamic_pointer_cast<KnifeBase>(obj->GetCurrentHeldWeapon());
			if (knife)
			{
				knife->AddAttackTrigger();
				m_has_trigger_created = true;
				EventSystem::GetInstance()->Publish(FirstSlashEvent(knife->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
			}
		}
	}

	// 攻撃判定用トリガーを削除
	if (!m_has_trigger_deleted)
	{
		if (play_rate > 0.8f && anim_kind == PlayerAnimKind::kFirstSideSlashKnife)
		{
			const auto knife = std::dynamic_pointer_cast<KnifeBase>(obj->GetCurrentHeldWeapon());
			if (knife)
			{
				knife->RemoveAttackTrigger();
				m_has_trigger_deleted = true;
			}
		}
	}
}

void player_state::FirstSideSlashKnife::LateUpdate()
{

}

void player_state::FirstSideSlashKnife::Enter()
{
	m_combo_timer			= 0.0f;
	m_has_trigger_created	= false;
	m_has_trigger_deleted	= false;

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::FirstSideSlashKnife::Exit()
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

int player_state::FirstSideSlashKnife::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 切り裂く(第二段階)
	if (obj->CanControl() && m_combo_timer > kComboValidTime && command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent))
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
