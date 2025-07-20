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
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleEquipKnifeped),						AnimPath.IDLE_EQUIP_KNIFE,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleAimKnife),							AnimPath.IDLE_AIM_KNIFE,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleStabKnife),							AnimPath.IDLE_STAB_KNIFE,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleSideSlashKnife),					AnimPath.IDLE_SIDE_SLASH_KNIFE,						0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleParry),								AnimPath.IDLE_PARRY,								0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleEquipGun),							AnimPath.IDLE_EQUIP_GUN,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleAimGun),							AnimPath.IDLE_AIM_GUN,								0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleShot),								AnimPath.IDLE_SHOT,									0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleReload),							AnimPath.IDLE_RELOAD,								0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouch),							AnimPath.IDLE_CROUCH,								0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchEquipKnifeped),				AnimPath.IDLE_CROUCH_EQUIP_KNIFE,					0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchEquipGun),					AnimPath.IDLE_RELOAD_GUN_EQUIPEED,					0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchReload),						AnimPath.IDLE_RELOAD_RELOAD,						0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleTurnAround),						AnimPath.IDLE_TURN_AROUND,							0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleTurnAroundEquipKnifeped),			AnimPath.IDLE_TURN_AROUND_EQUIP_KNIFE,				0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleTurnAroundEquipGun),				AnimPath.IDLE_TURN_AROUND_GUN_EQUIPEED,				0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAround),					AnimPath.IDLE_CROUCH_TURN_AROUND,					0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundEquipKnifeped),		AnimPath.IDLE_CROUCH_TURN_AROUND_EQUIP_KNIFE,		0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundEquipGun),			AnimPath.IDLE_CROUCH_TURN_AROUND_GUN_EQUIPEED,		0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForward),							AnimPath.MOVE_FORWARD,								0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackward),							AnimPath.MOVE_BACKWARD,								0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeft),								AnimPath.MOVE_LEFT,									0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRight),								AnimPath.MOVE_RIGHT,								0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeft),						AnimPath.MOVE_FORWARD_LEFT,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRight),						AnimPath.MOVE_FORWARD_RIGHT,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeft),						AnimPath.MOVE_BACKWARD_LEFT,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRight),						AnimPath.MOVE_BACKWARD_RIGHT,						0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardEquipKnifeped),				AnimPath.MOVE_FORWARD_EQUIP_KNIFE,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardEquipKnifeped),				AnimPath.MOVE_BACKWARD_EQUIP_KNIFE,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftEquipKnifeped),					AnimPath.MOVE_LEFT_EQUIP_KNIFE,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightEquipKnifeped),				AnimPath.MOVE_RIGHT_EQUIP_KNIFE,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftEquipKnifeped),			AnimPath.MOVE_FORWARD_LEFT_EQUIP_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightEquipKnifeped),			AnimPath.MOVE_FORWARD_RIGHT_EQUIP_KNIFE,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftEquipKnifeped),			AnimPath.MOVE_BACKWARD_LEFT_EQUIP_KNIFE,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightEquipKnifeped),		AnimPath.MOVE_BACKWARD_RIGHT_EQUIP_KNIFE,			0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardAimKnife),					AnimPath.MOVE_FORWARD_AIM_KNIFE,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardAimKnife),					AnimPath.MOVE_BACKWARD_AIM_KNIFE,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftAimKnife),						AnimPath.MOVE_LEFT_AIM_KNIFE,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightAimKnife),						AnimPath.MOVE_RIGHT_AIM_KNIFE,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftAimKnife),				AnimPath.MOVE_FORWARD_LEFT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightAimKnife),				AnimPath.MOVE_FORWARD_RIGHT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftAimKnife),				AnimPath.MOVE_BACKWARD_LEFT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightAimKnife),				AnimPath.MOVE_BACKWARD_RIGHT_AIM_KNIFE,				0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardEquipGun),					AnimPath.MOVE_FORWARD_EQUIP_GUN,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardEquipGun),					AnimPath.MOVE_BACKWARD_EQUIP_GUN,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftEquipGun),						AnimPath.MOVE_LEFT_EQUIP_GUN,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightEquipGun),						AnimPath.MOVE_RIGHT_EQUIP_GUN,						0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftEquipGun),				AnimPath.MOVE_FORWARD_LEFT_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightEquipGun),				AnimPath.MOVE_FORWARD_RIGHT_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftEquipGun),				AnimPath.MOVE_BACKWARD_LEFT_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightEquipGun),				AnimPath.MOVE_BACKWARD_RIGHT_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);

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

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchEquipKnifeped),		AnimPath.MOVE_FORWARD_CROUCH_EQUIP_KNIFE,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouchEquipKnifeped),		AnimPath.MOVE_BACKWARD_CROUCH_EQUIP_KNIFE,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouchEquipKnifeped),			AnimPath.MOVE_LEFT_CROUCH_EQUIP_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouchEquipKnifeped),			AnimPath.MOVE_RIGHT_CROUCH_EQUIP_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouchEquipKnifeped),	AnimPath.MOVE_FORWARD_LEFT_CROUCH_EQUIP_KNIFE,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouchEquipKnifeped),	AnimPath.MOVE_FORWARD_RIGHT_CROUCH_EQUIP_KNIFE,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouchEquipKnifeped),	AnimPath.MOVE_BACKWARD_LEFT_CROUCH_EQUIP_KNIFE,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouchEquipKnifeped),	AnimPath.MOVE_BACKWARD_RIGHT_CROUCH_EQUIP_KNIFE,	0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchEquipGun),				AnimPath.MOVE_FORWARD_CROUCH_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouchEquipGun),			AnimPath.MOVE_BACKWARD_CROUCH_EQUIP_GUN,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouchEquipGun),				AnimPath.MOVE_LEFT_CROUCH_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouchEquipGun),				AnimPath.MOVE_RIGHT_CROUCH_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouchEquipGun),			AnimPath.MOVE_FORWARD_LEFT_CROUCH_EQUIP_GUN,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouchEquipGun),		AnimPath.MOVE_FORWARD_RIGHT_CROUCH_EQUIP_GUN,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouchEquipGun),		AnimPath.MOVE_BACKWARD_LEFT_CROUCH_EQUIP_GUN,		0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouchEquipGun),		AnimPath.MOVE_BACKWARD_RIGHT_CROUCH_EQUIP_GUN,		0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchReload),				AnimPath.MOVE_FORWARD_CROUCH_RELOAD,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardCrouchReload),				AnimPath.MOVE_BACKWARD_CROUCH_RELOAD,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveLeftCrouchReload),					AnimPath.MOVE_LEFT_CROUCH_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveRightCrouchReload),					AnimPath.MOVE_RIGHT_CROUCH_RELOAD,					0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardLeftCrouchReload),			AnimPath.MOVE_FORWARD_LEFT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRightCrouchReload),			AnimPath.MOVE_FORWARD_RIGHT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardLeftCrouchReload),			AnimPath.MOVE_BACKWARD_LEFT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveBackwardRightCrouchReload),			AnimPath.MOVE_BACKWARD_RIGHT_CROUCH_RELOAD,			0, AnimTag.MOVE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRun),						AnimPath.MOVE_FORWARD_RUN,							0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRunEquipKnifeped),			AnimPath.MOVE_FORWARD_RUN_EQUIP_KNIFE,				0, AnimTag.MOVE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kMoveForwardRunEquipGun),				AnimPath.MOVE_FORWARD_RUN_EQUIP_GUN,				0, AnimTag.MOVE, 20.0f, true);
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
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleEquipKnifeped));
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

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleEquipGun));
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

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchEquipKnifeped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchEquipGun));
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

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleTurnAroundEquipKnifeped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleTurnAroundEquipGun));
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

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundEquipKnifeped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdleCrouchTurnAroundEquipGun));
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

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardEquipKnifeped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAimKnife):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardAimKnife));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardEquipGun));
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

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchEquipKnifeped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchEquipGun));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnimEightDir(static_cast<int>(PlayerAnimKind::kMoveForwardCrouchEquipGun));
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

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipKnife):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRunEquipKnifeped));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnife):
		// MEMO : ダッシュ切りをここで判定するかは検討中
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kEquipGun):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRunEquipGun));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):
		AttachAnim(static_cast<int>(PlayerAnimKind::kMoveForwardRunReload));
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
		AttachAnim(forward_anim_kind + 4);
		return;
	}
	// 右前方
	if (command->IsExecuting(CommandKind::kMoveUpPlayer)   && command->IsExecuting(CommandKind::kMoveRightPlayer))
	{
		AttachAnim(forward_anim_kind + 5);
		return;
	}
	// 左後方
	if (command->IsExecuting(CommandKind::kMoveDownPlayer) && command->IsExecuting(CommandKind::kMoveLeftPlayer))
	{
		AttachAnim(forward_anim_kind + 6);
		return;
	}
	// 右後方
	if (command->IsExecuting(CommandKind::kMoveDownPlayer) && command->IsExecuting(CommandKind::kMoveRightPlayer))
	{
		AttachAnim(forward_anim_kind + 7);
		return;
	}
	// 前方
	if (command->IsExecuting(CommandKind::kMoveUpPlayer))
	{
		AttachAnim(forward_anim_kind);
		return;
	}
	// 後方
	if (command->IsExecuting(CommandKind::kMoveDownPlayer))
	{
		AttachAnim(forward_anim_kind + 1);
		return;
	}
	// 左
	if (command->IsExecuting(CommandKind::kMoveLeftPlayer))
	{
		AttachAnim(forward_anim_kind + 2);
		return;
	}
	// 右
	if (command->IsExecuting(CommandKind::kMoveRightPlayer))
	{
		AttachAnim(forward_anim_kind + 3);
		return;
	}
}
