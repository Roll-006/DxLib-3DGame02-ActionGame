#include "player_state_controller.hpp"
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
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle),						AnimPath.IDLE,							0, AnimTag.NONE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleInjured),				AnimPath.IDLE_INJURED,					0, AnimTag.NONE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kDead),						AnimPath.DEAD_02,						0, AnimTag.NONE, 50.0f,  false);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kGrabbed),					AnimPath.GRABBED,						0, AnimTag.NONE, 30.0f,  true,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kFrontKick),					AnimPath.FRONT_KICK,					0, AnimTag.NONE, 80.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kRoundhouseKick),			AnimPath.ROUNDHOUSE_KICK,				0, AnimTag.NONE, 80.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEscapeKick),				AnimPath.ESCAPE_KICK,					0, AnimTag.NONE, 80.0f,  false);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kStabKnife),					AnimPath.STAB_KNIFE,					0, AnimTag.NONE, 100.0f, false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kFirstSideSlashKnife),		AnimPath.FIRST_SIDE_SLASH_KNIFE,		0, AnimTag.NONE, 50.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kSecondSideSlashKnife),		AnimPath.SECOND_SIDE_SLASH_KNIFE,		0, AnimTag.NONE, 75.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kSpinningSlashKnife),		AnimPath.SPINNING_SLASH_KNIFE,			0, AnimTag.NONE, 50.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kStealthKill),				AnimPath.STEALTH_KILL,					0, AnimTag.NONE, 60.0f,  false);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRun),			AnimPath.MOVE_FORWARD_RUN_01,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRunInjured),		AnimPath.MOVE_FORWARD_RUN_INJURED,		0, AnimTag.MOVE, 50.0f,  true);

	// 上半身用
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAttachRifle),				AnimPath.ATTACH_RIFLE,					0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kDetachRifle),				AnimPath.DETACH_RIFLE,					0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAttachHandgun),				AnimPath.ATTACH_HANDGUN,				0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kDetachHandgun),				AnimPath.DETACH_HANDGUN,				0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEquipKnife),				AnimPath.EQUIP_KNIFE,					0, AnimTag.NONE, 20.0f,  true,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAimKnife),					AnimPath.AIM_KNIFE,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kParry),						AnimPath.PARRY,							0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEquipGun),					AnimPath.EQUIP_GUN,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAimGun),					AnimPath.AIM_GUN,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kReload),					AnimPath.RELOAD,						0, AnimTag.NONE, 100.0f, false);

	// 下半身用
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kCrouch),					AnimPath.CROUCH,						0, AnimTag.NONE, 20.0f,  true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForward),				AnimPath.MOVE_FORWARD,					0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackward),				AnimPath.MOVE_BACKWARD,					0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeft),					AnimPath.MOVE_LEFT,						0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRight),					AnimPath.MOVE_RIGHT,					0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeft),			AnimPath.MOVE_FORWARD_LEFT,				0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRight),			AnimPath.MOVE_FORWARD_RIGHT,			0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeft),			AnimPath.MOVE_BACKWARD_LEFT,			0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRight),			AnimPath.MOVE_BACKWARD_RIGHT,			0, AnimTag.MOVE, 55.0f,  true);
	
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardInjured),		AnimPath.MOVE_FORWARD_INJURED,			0, AnimTag.MOVE, 50.0f,  true, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardInjured),		AnimPath.MOVE_BACKWARD_INJURED,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftInjured),			AnimPath.MOVE_LEFT_INJURED,				0, AnimTag.MOVE, 30.0f,  true, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightInjured),			AnimPath.MOVE_RIGHT_INJURED,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftInjured),	AnimPath.MOVE_FORWARD_LEFT_INJURED,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightInjured),	AnimPath.MOVE_FORWARD_RIGHT_INJURED,	0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftInjured),	AnimPath.MOVE_BACKWARD_LEFT_INJURED,	0, AnimTag.MOVE, 30.0f,  true, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightInjured),	AnimPath.MOVE_BACKWARD_RIGHT_INJURED,	0, AnimTag.MOVE, 50.0f,  true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouch),			AnimPath.MOVE_FORWARD_CROUCH,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouch),		AnimPath.MOVE_BACKWARD_CROUCH,			0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouch),			AnimPath.MOVE_LEFT_CROUCH,				0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouch),			AnimPath.MOVE_RIGHT_CROUCH,				0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouch),		AnimPath.MOVE_FORWARD_LEFT_CROUCH,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouch),	AnimPath.MOVE_FORWARD_RIGHT_CROUCH,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouch),	AnimPath.MOVE_BACKWARD_LEFT_CROUCH,		0, AnimTag.MOVE, 50.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouch),	AnimPath.MOVE_BACKWARD_RIGHT_CROUCH,	0, AnimTag.MOVE, 50.0f,  true);
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
		AttachAnim(idle_anim, BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),					BodyKind::kUpperBody);
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
