#include "player_state_controller.hpp"
#include "player_animator.hpp"

PlayerAnimator::PlayerAnimator(const std::shared_ptr<Modeler> modeler, const std::shared_ptr<PlayerStateController> state) :
	AnimatorBase(modeler),
	m_state		(state)
{
	LoadAnim();
}

PlayerAnimator::~PlayerAnimator()
{

}

void PlayerAnimator::Init()
{

}

void PlayerAnimator::Update()
{
	ChangeAnim();

	BlendAnim();
	PlayAnim();
}

void PlayerAnimator::LoadAnim()
{
	// 上半身
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEquipKnife),				AnimPath.EQUIP_KNIFE,					0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAimKnife),					AnimPath.AIM_KNIFE,						1, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kStabKnife),					AnimPath.STAB_KNIFE,					1, AnimTag.NONE, 100.0f, false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kFirstSideSlashKnife),		AnimPath.FIRST_SIDE_SLASH_KNIFE,		0, AnimTag.NONE, 100.0f, false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kSecondSideSlashKnife),		AnimPath.SECOND_SIDE_SLASH_KNIFE,		0, AnimTag.NONE, 70.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kParry),						AnimPath.PARRY,							0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kEquipGun),					AnimPath.EQUIP_GUN,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kAimGun),					AnimPath.AIM_GUN,						0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kReload),					AnimPath.RELOAD,						1, AnimTag.NONE, 100.0f, false);

	// 下半身
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle),						AnimPath.IDLE,							0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kCrouch),					AnimPath.CROUCH,						0, AnimTag.NONE, 20.0f,  true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForward),				AnimPath.MOVE_FORWARD,					0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackward),				AnimPath.MOVE_BACKWARD,					0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeft),					AnimPath.MOVE_LEFT,						0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRight),					AnimPath.MOVE_RIGHT,					0, AnimTag.MOVE, 30.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeft),			AnimPath.MOVE_FORWARD_LEFT,				0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRight),			AnimPath.MOVE_FORWARD_RIGHT,			0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeft),			AnimPath.MOVE_BACKWARD_LEFT,			0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRight),			AnimPath.MOVE_BACKWARD_RIGHT,			0, AnimTag.MOVE, 20.0f,  true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouch),			AnimPath.MOVE_FORWARD_CROUCH,			0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouch),		AnimPath.MOVE_BACKWARD_CROUCH,			0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouch),			AnimPath.MOVE_LEFT_CROUCH,				0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouch),			AnimPath.MOVE_RIGHT_CROUCH,				0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouch),		AnimPath.MOVE_FORWARD_LEFT_CROUCH,		0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouch),	AnimPath.MOVE_FORWARD_RIGHT_CROUCH,		0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouch),	AnimPath.MOVE_BACKWARD_LEFT_CROUCH,		0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouch),	AnimPath.MOVE_BACKWARD_RIGHT_CROUCH,	0, AnimTag.MOVE, 20.0f,  true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRun),			AnimPath.MOVE_FORWARD_RUN,				0, AnimTag.MOVE, 27.0f,  true);
}

void PlayerAnimator::ChangeAnim()
{
	switch (m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::MoveStateKind::kMoveNull):
		CombineMoveNullWithAction();
		break;

	case static_cast<int>(player_state::MoveStateKind::kMove):
		CombineMoveWithAction();
		break;

	default:
		break;
	}
}


#pragma region 状態の合成
void PlayerAnimator::CombineMoveNullWithAction()
{
	switch (m_state->GetActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::ActionStateKind::kActionNull):
		CombineMoveNullActionNullWithWeaponAction();
		break;

	case static_cast<int>(player_state::ActionStateKind::kCrouch):
		CombineMoveNullCrouchWithWeaponAction();
		break;

	case static_cast<int>(player_state::ActionStateKind::kTurnAround):
		CombineMoveNullTurnAroundWithWeaponAction();
		break;

	case static_cast<int>(player_state::ActionStateKind::kCrouchTurnAround):
		CombineMoveNullCrouchTurnAroundWithWeaponAction();
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveWithAction()
{
	switch (m_state->GetActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::ActionStateKind::kActionNull):
		CombineMoveActionNullWithWeaponAction();
		break;

	case static_cast<int>(player_state::ActionStateKind::kCrouch):
		CombineMoveCrouchWithWeaponAction();
		break;

	case static_cast<int>(player_state::ActionStateKind::kRun):
		CombineMoveRunWithWeaponAction();
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullActionNullWithWeaponAction()
{
	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kIdle));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),							BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),					BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),							BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimKnife),						BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kStabKnife):
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kStabKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kFirstSideSlashKnife):
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kFirstSideSlashKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSecondSideSlashKnife):
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kSecondSideSlashKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kParry):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),							BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kParry),						BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),							BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),						BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimGun):
	case static_cast<int>(player_state::WeaponActionStateKind::kShot):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),							BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimGun),						BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),							BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),						BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullCrouchWithWeaponAction()
{
	AttachAnim(static_cast<int>(PlayerAnimKind::kCrouch), BodyKind::kLowerBody);

	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),			BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),	BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),		BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),		BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullTurnAroundWithWeaponAction()
{
	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullCrouchTurnAroundWithWeaponAction()
{
	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveActionNullWithWeaponAction()
{
	AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward));

	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),			BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),	BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimKnife),		BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),		BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimGun):
	case static_cast<int>(player_state::WeaponActionStateKind::kShot):
		AttachAnim(static_cast<int>(PlayerAnimKind::kAimGun),		BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),		BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveCrouchWithWeaponAction()
{
	AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouch));

	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle),			BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),	BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),		BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),		BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveRunWithWeaponAction()
{
	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachResultAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRun));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRun),	BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipKnife),		BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kFirstSideSlashKnife):
		// MEMO : ダッシュ切りをここで判定するかは検討中
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRun),	BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kEquipGun),			BodyKind::kUpperBody);
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRun),	BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(PlayerAnimKind::kReload),			BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}
#pragma endregion


void PlayerAnimator::AttachAnimEightDir(const int forward_anim_kind)
{
	const auto command = CommandHandler::GetInstance();

	// forwardのアニメーションを基準とし、enum classの値をずらしてアタッチ
	// 左前方
	if (command->IsExecuting(CommandKind::kMoveUpPlayer)   && command->IsExecuting(CommandKind::kMoveLeftPlayer))
	{
		AttachAnim(forward_anim_kind + 4, BodyKind::kLowerBody);
		return;
	}
	// 右前方
	if (command->IsExecuting(CommandKind::kMoveUpPlayer)   && command->IsExecuting(CommandKind::kMoveRightPlayer))
	{
		AttachAnim(forward_anim_kind + 5, BodyKind::kLowerBody);
		return;
	}
	// 左後方
	if (command->IsExecuting(CommandKind::kMoveDownPlayer) && command->IsExecuting(CommandKind::kMoveLeftPlayer))
	{
		AttachAnim(forward_anim_kind + 6, BodyKind::kLowerBody);
		return;
	}
	// 右後方
	if (command->IsExecuting(CommandKind::kMoveDownPlayer) && command->IsExecuting(CommandKind::kMoveRightPlayer))
	{
		AttachAnim(forward_anim_kind + 7, BodyKind::kLowerBody);
		return;
	}
	// 前方
	if (command->IsExecuting(CommandKind::kMoveUpPlayer))
	{
		AttachAnim(forward_anim_kind, BodyKind::kLowerBody);
		return;
	}
	// 後方
	if (command->IsExecuting(CommandKind::kMoveDownPlayer))
	{
		AttachAnim(forward_anim_kind + 1, BodyKind::kLowerBody);
		return;
	}
	// 左
	if (command->IsExecuting(CommandKind::kMoveLeftPlayer))
	{
		AttachAnim(forward_anim_kind + 2, BodyKind::kLowerBody);
		return;
	}
	// 右
	if (command->IsExecuting(CommandKind::kMoveRightPlayer))
	{
		AttachAnim(forward_anim_kind + 3, BodyKind::kLowerBody);
		return;
	}
}
