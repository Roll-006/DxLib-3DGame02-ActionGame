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
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle),									AnimPath.IDLE,										0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleKnifeEquipped),						AnimPath.IDLE_KNIFE_EQUIPPED,						0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleAimKnife),							AnimPath.IDLE_AIM_KNIFE,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleStabKnife),							AnimPath.IDLE_STAB_KNIFE,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleSideSlashKnife),					AnimPath.IDLE_SIDE_SLASH_KNIFE,						0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleParry),								AnimPath.IDLE_PARRY,								0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleGunEquipped),						AnimPath.IDLE_GUN_EQUIPPED,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleAimGun),							AnimPath.IDLE_AIM_GUN,								0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleShot),								AnimPath.IDLE_SHOT,									0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleReload),							AnimPath.IDLE_RELOAD,								0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouch),							AnimPath.IDLE_CROUCH,								0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchKnifeEquipped),				AnimPath.IDLE_CROUCH_KNIFE_EQUIPPED,				0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchGunEquipped),					AnimPath.IDLE_RELOAD_GUN_EQUIPEED,					0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchReload),						AnimPath.IDLE_RELOAD_RELOAD,						0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleTurnAround),						AnimPath.IDLE_TURN_AROUND,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleTurnAroundKnifeEquipped),			AnimPath.IDLE_TURN_AROUND_KNIFE_EQUIPPED,			0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleTurnAroundGunEquipped),				AnimPath.IDLE_TURN_AROUND_GUN_EQUIPEED,				0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAround),					AnimPath.IDLE_CROUCH_TURN_AROUND,					0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundKnifeEquipped),		AnimPath.IDLE_CROUCH_TURN_AROUND_KNIFE_EQUIPPED,	0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundGunEquipped),		AnimPath.IDLE_CROUCH_TURN_AROUND_GUN_EQUIPEED,		0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForward),							AnimPath.MOVE_FORWARD,								0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackward),							AnimPath.MOVE_BACKWARD,								0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeft),								AnimPath.MOVE_LEFT,									0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRight),								AnimPath.MOVE_RIGHT,								0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeft),						AnimPath.MOVE_FORWARD_LEFT,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRight),						AnimPath.MOVE_FORWARD_RIGHT,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeft),						AnimPath.MOVE_BACKWARD_LEFT,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRight),						AnimPath.MOVE_BACKWARD_RIGHT,						0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardKnifeEquipped),				AnimPath.MOVE_FORWARD_KNIFE_EQUIPPED,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardKnifeEquipped),				AnimPath.MOVE_BACKWARD_KNIFE_EQUIPPED,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftKnifeEquipped),					AnimPath.MOVE_LEFT_KNIFE_EQUIPPED,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightKnifeEquipped),				AnimPath.MOVE_RIGHT_KNIFE_EQUIPPED,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftKnifeEquipped),			AnimPath.MOVE_FORWARD_LEFT_KNIFE_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightKnifeEquipped),			AnimPath.MOVE_FORWARD_RIGHT_KNIFE_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftKnifeEquipped),			AnimPath.MOVE_BACKWARD_LEFT_KNIFE_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightKnifeEquipped),		AnimPath.MOVE_BACKWARD_RIGHT_KNIFE_EQUIPPED,		0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardAimKnife),					AnimPath.MOVE_FORWARD_AIM_KNIFE,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardAimKnife),					AnimPath.MOVE_BACKWARD_AIM_KNIFE,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftAimKnife),						AnimPath.MOVE_LEFT_AIM_KNIFE,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightAimKnife),						AnimPath.MOVE_RIGHT_AIM_KNIFE,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftAimKnife),				AnimPath.MOVE_FORWARD_LEFT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightAimKnife),				AnimPath.MOVE_FORWARD_RIGHT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftAimKnife),				AnimPath.MOVE_BACKWARD_LEFT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightAimKnife),				AnimPath.MOVE_BACKWARD_RIGHT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardGunEquipped),				AnimPath.MOVE_FORWARD_GUN_EQUIPPED,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardGunEquipped),				AnimPath.MOVE_BACKWARD_GUN_EQUIPPED,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftGunEquipped),					AnimPath.MOVE_LEFT_GUN_EQUIPPED,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightGunEquipped),					AnimPath.MOVE_RIGHT_GUN_EQUIPPED,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftGunEquipped),			AnimPath.MOVE_FORWARD_LEFT_GUN_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightGunEquipped),			AnimPath.MOVE_FORWARD_RIGHT_GUN_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftGunEquipped),			AnimPath.MOVE_BACKWARD_LEFT_GUN_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightGunEquipped),			AnimPath.MOVE_BACKWARD_RIGHT_GUN_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardAimGun),						AnimPath.MOVE_FORWARD_AIM_GUN,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardAimGun),					AnimPath.MOVE_BACKWARD_AIM_GUN,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftAimGun),						AnimPath.MOVE_LEFT_AIM_GUN,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightAimGun),						AnimPath.MOVE_RIGHT_AIM_GUNE,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftAimGun),					AnimPath.MOVE_FORWARD_LEFT_AIM_GUN,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightAimGun),				AnimPath.MOVE_FORWARD_RIGHT_AIM_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftAimGun),				AnimPath.MOVE_BACKWARD_LEFT_AIM_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightAimGun),				AnimPath.MOVE_BACKWARD_RIGHT_AIM_GUN,				0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardShot),						AnimPath.MOVE_FORWARD_SHOT,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardShot),						AnimPath.MOVE_BACKWARD_SHOT,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftShot),							AnimPath.MOVE_LEFT_SHOT,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightShot),							AnimPath.MOVE_RIGHT_SHOT,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftShot),					AnimPath.MOVE_FORWARD_LEFT_SHOT,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightShot),					AnimPath.MOVE_FORWARD_RIGHT_SHOT,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftShot),					AnimPath.MOVE_BACKWARD_LEFT_SHOT,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightShot),					AnimPath.MOVE_BACKWARD_RIGHT_SHOT,					0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardReload),						AnimPath.MOVE_FORWARD_RELOAD,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardReload),					AnimPath.MOVE_BACKWARD_RELOAD,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftReload),						AnimPath.MOVE_LEFT_RELOAD,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightReload),						AnimPath.MOVE_RIGHT_RELOAD,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftReload),					AnimPath.MOVE_FORWARD_LEFT_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightReload),				AnimPath.MOVE_FORWARD_RIGHT_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftReload),				AnimPath.MOVE_BACKWARD_LEFT_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightReload),				AnimPath.MOVE_BACKWARD_RIGHT_RELOAD,				0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouch),						AnimPath.MOVE_FORWARD_CROUCH,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouch),					AnimPath.MOVE_BACKWARD_CROUCH,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouch),						AnimPath.MOVE_LEFT_CROUCH,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouch),						AnimPath.MOVE_RIGHT_CROUCH,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouch),					AnimPath.MOVE_FORWARD_LEFT_CROUCH,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouch),				AnimPath.MOVE_FORWARD_RIGHT_CROUCH,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouch),				AnimPath.MOVE_BACKWARD_LEFT_CROUCH,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouch),				AnimPath.MOVE_BACKWARD_RIGHT_CROUCH,				0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchKnifeEquipped),		AnimPath.MOVE_FORWARD_CROUCH_KNIFE_EQUIPPED,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouchKnifeEquipped),		AnimPath.MOVE_BACKWARD_CROUCH_KNIFE_EQUIPPED,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouchKnifeEquipped),			AnimPath.MOVE_LEFT_CROUCH_KNIFE_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouchKnifeEquipped),			AnimPath.MOVE_RIGHT_CROUCH_KNIFE_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouchKnifeEquipped),	AnimPath.MOVE_FORWARD_LEFT_CROUCH_KNIFE_EQUIPPED,	0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouchKnifeEquipped),	AnimPath.MOVE_FORWARD_RIGHT_CROUCH_KNIFE_EQUIPPED,	0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouchKnifeEquipped),	AnimPath.MOVE_BACKWARD_LEFT_CROUCH_KNIFE_EQUIPPED,	0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouchKnifeEquipped),	AnimPath.MOVE_BACKWARD_RIGHT_CROUCH_KNIFE_EQUIPPED, 0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchGunEquipped),			AnimPath.MOVE_FORWARD_CROUCH_GUN_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouchGunEquipped),			AnimPath.MOVE_BACKWARD_CROUCH_GUN_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouchGunEquipped),				AnimPath.MOVE_LEFT_CROUCH_GUN_EQUIPPED,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouchGunEquipped),			AnimPath.MOVE_RIGHT_CROUCH_GUN_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouchGunEquipped),		AnimPath.MOVE_FORWARD_LEFT_CROUCH_GUN_EQUIPPED,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouchGunEquipped),		AnimPath.MOVE_FORWARD_RIGHT_CROUCH_GUN_EQUIPPED,	0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouchGunEquipped),		AnimPath.MOVE_BACKWARD_LEFT_CROUCH_GUN_EQUIPPED,	0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouchGunEquipped),	AnimPath.MOVE_BACKWARD_RIGHT_CROUCH_GUN_EQUIPPED,	0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchReload),				AnimPath.MOVE_FORWARD_CROUCH_RELOAD,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouchReload),				AnimPath.MOVE_BACKWARD_CROUCH_RELOAD,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouchReload),					AnimPath.MOVE_LEFT_CROUCH_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouchReload),					AnimPath.MOVE_RIGHT_CROUCH_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouchReload),			AnimPath.MOVE_FORWARD_LEFT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouchReload),			AnimPath.MOVE_FORWARD_RIGHT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouchReload),			AnimPath.MOVE_BACKWARD_LEFT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouchReload),			AnimPath.MOVE_BACKWARD_RIGHT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRun),						AnimPath.MOVE_FORWARD_RUN,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRunKnifeEquipped),			AnimPath.MOVE_FORWARD_RUN_KNIFE_EQUIPPED,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRunGunEquipped),				AnimPath.MOVE_FORWARD_RUN_GUN_EQUIPPED,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRunReload),					AnimPath.MOVE_FORWARD_RUN_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
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
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleKnifeEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleAimKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kStabKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleStabKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleSideSlashKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kParry):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleParry));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleGunEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleAimGun));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kShot):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleShot));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleReload));
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveNullCrouchWithWeaponAction()
{
	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouch));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchKnifeEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchGunEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchReload));
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
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleTurnAround));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleTurnAroundKnifeEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleTurnAroundGunEquipped));
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
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAround));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundKnifeEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundGunEquipped));
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveActionNullWithWeaponAction()
{
	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForward));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardKnifeEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimKnife):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardAimKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardGunEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimGun):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardAimGun));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kShot):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardShot));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardReload));
		break;

	default:
		break;
	}
}

void PlayerAnimator::CombineMoveCrouchWithWeaponAction()
{
	switch (m_state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouch));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchKnifeEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchGunEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchGunEquipped));
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
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRun));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRunKnifeEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnife):
		// MEMO : ダッシュ切りをここで判定するかは検討中
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRunGunEquipped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRunReload));
		break;

	default:
		break;
	}
}

void PlayerAnimator::AttachAnimEightDir(const int forward_anim_kind)
{
	// forward移動のアニメーションを基準とし、enum classの値をずらしてアタッチ
	//// 左前方
	//if ()
	//{
	//	AttachAnim(forward_anim_kind + 4);
	//}
	//// 右前方
	//if ()
	//{
	//	AttachAnim(forward_anim_kind + 5);
	//}
	//// 左後方
	//if ()
	//{
	//	AttachAnim(forward_anim_kind + 6);
	//}
	//// 右後方
	//if ()
	//{
	//	AttachAnim(forward_anim_kind + 7);
	//}
	//// 前方
	//if ()
	//{
	//	AttachAnim(forward_anim_kind);
	//}
	//// 後方
	//if ()
	//{
	//	AttachAnim(forward_anim_kind + 1);
	//}
	//// 左
	//if ()
	//{
	//	AttachAnim(forward_anim_kind + 2);
	//}
	//// 右
	//if ()
	//{
	//	AttachAnim(forward_anim_kind + 3);
	//}
}
