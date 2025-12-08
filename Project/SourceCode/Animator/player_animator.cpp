#include "../Part/player_state_controller.hpp"
#include "player_animator.hpp"

PlayerAnimator::PlayerAnimator(
	const std::shared_ptr<Modeler>&					modeler,
	const std::shared_ptr<PlayerStateController>&	state,
	const std::shared_ptr<WeaponBase>&				held_weapon,
	const std::shared_ptr<WeaponBase>&				equip_weapon) :
	AnimatorBase	(modeler, ObjName.PLAYER),
	m_state			(state),
	m_held_weapon	(held_weapon),
	m_equip_weapon	(equip_weapon),
	m_is_near_death	(false)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<NearDeathEvent>(this, &PlayerAnimator::ActivateNearDeathAnimation);

	LoadAnim();
}

PlayerAnimator::~PlayerAnimator()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<NearDeathEvent>(this, &PlayerAnimator::ActivateNearDeathAnimation);
}

void PlayerAnimator::Init()
{

}

void PlayerAnimator::Update()
{
	DivideBone();
	ChangeAnim();

	BlendAnim();
	PlayAnim();

	m_is_near_death = false;
}

void PlayerAnimator::ActivateNearDeathAnimation(const NearDeathEvent& event)
{
	m_is_near_death = true;
}

void PlayerAnimator::LoadAnim()
{
	// 汎用
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle),						SWATAnimPath.IDLE,							0, AnimTag.NONE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleInjured),				SWATAnimPath.IDLE_INJURED,					0, AnimTag.NONE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kStandToCrouch),				SWATAnimPath.STAND_TO_CROUCH,				0, AnimTag.NONE, 50.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kDead),						SWATAnimPath.DEAD_02,						0, AnimTag.NONE, 50.0f,  false);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kGrabbed),					SWATAnimPath.GRABBED,						0, AnimTag.NONE, 30.0f,  true,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kFrontKick),					SWATAnimPath.FRONT_KICK,					0, AnimTag.NONE, 80.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kRoundhouseKick),			SWATAnimPath.ROUNDHOUSE_KICK,				0, AnimTag.NONE, 80.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEscapeKick),				SWATAnimPath.ESCAPE_KICK,					0, AnimTag.NONE, 80.0f,  false);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kStabKnife),					SWATAnimPath.STAB_KNIFE,					0, AnimTag.NONE, 100.0f, false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kFirstSideSlashKnife),		SWATAnimPath.FIRST_SIDE_SLASH_KNIFE,		0, AnimTag.NONE, 70.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kSecondSideSlashKnife),		SWATAnimPath.SECOND_SIDE_SLASH_KNIFE,		0, AnimTag.NONE, 88.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kSpinningSlashKnife),		SWATAnimPath.SPINNING_SLASH_KNIFE,			0, AnimTag.NONE, 70.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kStealthKill),				SWATAnimPath.STEALTH_KILL,					0, AnimTag.NONE, 60.0f,  false);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRun),			SWATAnimPath.MOVE_FORWARD_RUN_01,			0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRunInjured),		SWATAnimPath.MOVE_FORWARD_RUN_INJURED,		0, AnimTag.MOVE, 50.0f,  true);

	// 上半身用
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAttachRifle),				SWATAnimPath.ATTACH_RIFLE,					0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kDetachRifle),				SWATAnimPath.DETACH_RIFLE,					0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAttachHandgun),				SWATAnimPath.ATTACH_HANDGUN,				0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kDetachHandgun),				SWATAnimPath.DETACH_HANDGUN,				0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEquipKnife),				SWATAnimPath.EQUIP_KNIFE,					0, AnimTag.NONE, 20.0f,  true, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAimKnife),					SWATAnimPath.AIM_KNIFE,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kParry),						SWATAnimPath.PARRY,							0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEquipGun),					SWATAnimPath.EQUIP_GUN,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAimGun),					SWATAnimPath.AIM_GUN,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kReload),					SWATAnimPath.RELOAD,						0, AnimTag.NONE, 100.0f, false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kTalkingOnPhone),			SWATAnimPath.TALKING_ON_PHONE,				0, AnimTag.NONE, 30.0f,  true, true);

	// 下半身用
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kCrouch),					SWATAnimPath.CROUCH,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kTalkingCrouch),				SWATAnimPath.TALKING_CROUCH,				0, AnimTag.NONE, 30.0f,  true, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForward),				SWATAnimPath.MOVE_FORWARD,					0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackward),				SWATAnimPath.MOVE_BACKWARD,					0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeft),					SWATAnimPath.MOVE_LEFT,						0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRight),					SWATAnimPath.MOVE_RIGHT,					0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeft),			SWATAnimPath.MOVE_FORWARD_LEFT,				0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRight),			SWATAnimPath.MOVE_FORWARD_RIGHT,			0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeft),			SWATAnimPath.MOVE_BACKWARD_LEFT,			0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRight),			SWATAnimPath.MOVE_BACKWARD_RIGHT,			0, AnimTag.MOVE, 55.0f,  true);
	
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardInjured),		SWATAnimPath.MOVE_FORWARD_INJURED,			0, AnimTag.MOVE, 50.0f,  true, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardInjured),		SWATAnimPath.MOVE_BACKWARD_INJURED,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftInjured),			SWATAnimPath.MOVE_LEFT_INJURED,				0, AnimTag.MOVE, 30.0f,  true, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightInjured),			SWATAnimPath.MOVE_RIGHT_INJURED,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftInjured),	SWATAnimPath.MOVE_FORWARD_LEFT_INJURED,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightInjured),	SWATAnimPath.MOVE_FORWARD_RIGHT_INJURED,	0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftInjured),	SWATAnimPath.MOVE_BACKWARD_LEFT_INJURED,	0, AnimTag.MOVE, 30.0f,  true, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightInjured),	SWATAnimPath.MOVE_BACKWARD_RIGHT_INJURED,	0, AnimTag.MOVE, 50.0f,  true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouch),			SWATAnimPath.MOVE_FORWARD_CROUCH,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouch),		SWATAnimPath.MOVE_BACKWARD_CROUCH,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouch),			SWATAnimPath.MOVE_LEFT_CROUCH,				0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouch),			SWATAnimPath.MOVE_RIGHT_CROUCH,				0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouch),		SWATAnimPath.MOVE_FORWARD_LEFT_CROUCH,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouch),	SWATAnimPath.MOVE_FORWARD_RIGHT_CROUCH,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouch),	SWATAnimPath.MOVE_BACKWARD_LEFT_CROUCH,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouch),	SWATAnimPath.MOVE_BACKWARD_RIGHT_CROUCH,	0, AnimTag.MOVE, 50.0f,  true);
}

void PlayerAnimator::ChangeAnim()
{
	switch (static_cast<player_state::MoveStateKind>(m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::MoveStateKind::kIdle:
		CombineMoveNullWithAction();
		break;

	case player_state::MoveStateKind::kMove:
		CombineMoveWithAction();
		break;

	default:
		break;
	}
}


#pragma region 状態の合成
void PlayerAnimator::CombineMoveNullWithAction()
{
	const auto current_lower_anim_kind = GetAnimKind(BodyKind::kLowerBody, TimeKind::kCurrent);

	switch (static_cast<player_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::ActionStateKind::kActionNull:
		CombineMoveNullActionNullWithWeaponAction();
		break;

	case player_state::ActionStateKind::kCrouch:
		CombineMoveNullCrouchWithWeaponAction();
		break;

	case player_state::ActionStateKind::kTurnAround:
		CombineMoveNullTurnAroundWithWeaponAction();
		break;

	case player_state::ActionStateKind::kCrouchTurnAround:
		CombineMoveNullCrouchTurnAroundWithWeaponAction();
		break;

	case player_state::ActionStateKind::kDead:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kDead));
		break;

	case player_state::ActionStateKind::kGrabbed:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kGrabbed));
		break;

	case player_state::ActionStateKind::kFrontKick:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kFrontKick));
		break;

	case player_state::ActionStateKind::kRoundhouseKick:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kRoundhouseKick));
		break;

	case player_state::ActionStateKind::kEscape:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kEscapeKick));
		break;

	case player_state::ActionStateKind::kStealthKill:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kStealthKill));
		break;

	case player_state::ActionStateKind::kVictoryPose:
		if (current_lower_anim_kind == static_cast<int>(PlayerAnimKind::kStandToCrouch) || current_lower_anim_kind == static_cast<int>(PlayerAnimKind::kTalkingCrouch))
		{
			if (IsPlayEnd(BodyKind::kLowerBody))
			{
				AttachAnim(static_cast<int>(PlayerAnimKind::kTalkingCrouch),  BodyKind::kLowerBody);
				AttachAnim(static_cast<int>(PlayerAnimKind::kTalkingOnPhone), BodyKind::kUpperBody);
			}
		}
		else
		{
			AttachResultAnim(static_cast<int>(PlayerAnimKind::kStandToCrouch));
		}
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveWithAction()
{
	switch (static_cast<player_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::ActionStateKind::kActionNull:
		CombineMoveActionNullWithWeaponAction();
		break;

	case player_state::ActionStateKind::kCrouch:
		CombineMoveCrouchWithWeaponAction();
		break;

	case player_state::ActionStateKind::kRun:
		CombineMoveRunWithWeaponAction();
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullActionNullWithWeaponAction()
{
	const auto idle_anim = m_is_near_death ? static_cast<int>(PlayerAnimKind::kIdleInjured) : static_cast<int>(PlayerAnimKind::kIdle);

	switch (static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::WeaponActionStateKind::kWeaponActionNull:
		AttachResultAnim(idle_anim);
		break;

	case player_state::WeaponActionStateKind::kAttachWeapon:
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAttachHandgun),				BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kDetachWeapon:
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kDetachHandgun),				BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipKnife:
		if (m_is_near_death)
		{
			AttachResultAnim(idle_anim);
		}
		else
		{
			AttachAnim(idle_anim, BodyKind::kLowerBody);
			AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),				BodyKind::kUpperBody);
		}
		break;

	case player_state::WeaponActionStateKind::kAimKnife:
		DivideBone(BonePath.HIPS);
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimKnife),						BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kStabKnife:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kStabKnife));
		break;

	case player_state::WeaponActionStateKind::kFirstSideSlashKnife:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kFirstSideSlashKnife));
		break;

	case player_state::WeaponActionStateKind::kSecondSideSlashKnife:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kSecondSideSlashKnife));
		break;

	case player_state::WeaponActionStateKind::kSpinningSlashKnife:
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kSpinningSlashKnife));
		break;

	case player_state::WeaponActionStateKind::kParry:
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kParry),						BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipGun:
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),						BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kAimGun:
	case player_state::WeaponActionStateKind::kShot:
	case player_state::WeaponActionStateKind::kShotRocketLauncher:
		DivideBone(BonePath.HIPS);
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimGun),						BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kReload:
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),						BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullCrouchWithWeaponAction()
{
	AttachAnim(static_cast<int>(PlayerAnimKind::kCrouch), BodyKind::kLowerBody);

	switch (static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::WeaponActionStateKind::kWeaponActionNull:
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),				BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kAttachWeapon:
		AttachAnim(static_cast<int>(PlayerAnimKind::kAttachHandgun),	BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kDetachWeapon:
		AttachAnim(static_cast<int>(PlayerAnimKind::kDetachHandgun),	BodyKind::kUpperBody);
		break;


	case player_state::WeaponActionStateKind::kEquipKnife:
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),		BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipGun:
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),			BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kReload:
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),			BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullTurnAroundWithWeaponAction()
{
	switch (static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::WeaponActionStateKind::kWeaponActionNull:
		break;

	case player_state::WeaponActionStateKind::kEquipKnife:
		break;

	case player_state::WeaponActionStateKind::kEquipGun:
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullCrouchTurnAroundWithWeaponAction()
{
	switch (static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::WeaponActionStateKind::kWeaponActionNull:
		break;

	case player_state::WeaponActionStateKind::kEquipKnife:
		break;

	case player_state::WeaponActionStateKind::kEquipGun:
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveActionNullWithWeaponAction()
{
	const auto offset = m_is_near_death ? 8 : 0;

	switch (static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::WeaponActionStateKind::kWeaponActionNull:
		if (m_is_near_death)
		{
			AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, true);
		}
		else
		{
			AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
			AttachAnim(static_cast<int>(PlayerAnimKind::kIdle), BodyKind::kUpperBody);
		}
		break;

	case player_state::WeaponActionStateKind::kAttachWeapon:
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAttachHandgun),	BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kDetachWeapon:
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
		AttachAnim(static_cast<int>(PlayerAnimKind::kDetachHandgun),	BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipKnife:
		if (m_is_near_death)
		{
			AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, true);
		}
		else
		{
			//DivideBone(BonePath.HIPS);
			AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
			AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife), BodyKind::kUpperBody);
		}

		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
		break;

	case player_state::WeaponActionStateKind::kAimKnife:
		DivideBone(BonePath.HIPS);
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimKnife), BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipGun:
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),			BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kAimGun:
	case player_state::WeaponActionStateKind::kShot:
	case player_state::WeaponActionStateKind::kShotRocketLauncher:
		DivideBone(BonePath.HIPS);
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimGun),			BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kReload:
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward) + offset, false);
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),			BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveCrouchWithWeaponAction()
{
	AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouch), false);

	switch (static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::WeaponActionStateKind::kWeaponActionNull:
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),				BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kAttachWeapon:
		AttachAnim(static_cast<int>(PlayerAnimKind::kAttachHandgun),	BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kDetachWeapon:
		AttachAnim(static_cast<int>(PlayerAnimKind::kDetachHandgun),	BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipKnife:
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),		BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipGun:
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),			BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kReload:
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),			BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveRunWithWeaponAction()
{
	const auto run_anim = m_is_near_death ? static_cast<int>(PlayerAnimKind::kMoveForwardRunInjured) : static_cast<int>(PlayerAnimKind::kMoveForwardRun);
	
	switch (static_cast<player_state::WeaponActionStateKind>(m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case player_state::WeaponActionStateKind::kWeaponActionNull:
		AttachResultAnim(run_anim);
		break;

	case player_state::WeaponActionStateKind::kAttachWeapon:
		AttachAnim(run_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAttachHandgun),	BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kDetachWeapon:
		AttachAnim(run_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kDetachHandgun),	BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kEquipKnife:
		AttachResultAnim(run_anim);
		break;

	case player_state::WeaponActionStateKind::kEquipGun:
		AttachAnim(run_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),			BodyKind::kUpperBody);
		break;

	case player_state::WeaponActionStateKind::kReload:
		AttachAnim(run_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),			BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}
#pragma endregion


void PlayerAnimator::AttachAnimEightDir(const int forward_anim_kind, const bool is_result_attach)
{
	const auto	command	= CommandHandler::GetInstance();
	auto		offset	= 0;

	// forwardのアニメーションを基準とし、enum classの値をずらしてアタッチ
	// WARNING : enum依存なため順番の入れ替えには注意
	
	// 左前方
	if (   command->IsExecute(CommandKind::kMoveUpPlayer,   TimeKind::kCurrent)
		&& command->IsExecute(CommandKind::kMoveLeftPlayer, TimeKind::kCurrent))
	{
		offset = 4;
	}
	// 右前方
	else if (command->IsExecute(CommandKind::kMoveUpPlayer,    TimeKind::kCurrent)
		  && command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent))
	{
		offset = 5;
	}
	// 左後方
	else if (command->IsExecute(CommandKind::kMoveDownPlayer, TimeKind::kCurrent)
		  && command->IsExecute(CommandKind::kMoveLeftPlayer, TimeKind::kCurrent))
	{
		offset = 6;
	}
	// 右後方
	else if (command->IsExecute(CommandKind::kMoveDownPlayer,  TimeKind::kCurrent)
		  && command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent))
	{
		offset = 7;
	}
	// 前方
	else if (command->IsExecute(CommandKind::kMoveUpPlayer, TimeKind::kCurrent))
	{
		offset = 0;
	}
	// 後方
	else if (command->IsExecute(CommandKind::kMoveDownPlayer, TimeKind::kCurrent))
	{
		offset = 1;
	}
	// 左
	else if (command->IsExecute(CommandKind::kMoveLeftPlayer, TimeKind::kCurrent))
	{
		offset = 2;
	}
	// 右
	else if (command->IsExecute(CommandKind::kMoveRightPlayer, TimeKind::kCurrent))
	{
		offset = 3;
	}

	if (is_result_attach)
	{
		AttachResultAnim(forward_anim_kind + offset);
	}
	else
	{
		AttachAnim(forward_anim_kind + offset, BodyKind::kLowerBody);
	}
}
