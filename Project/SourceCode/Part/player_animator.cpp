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
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle01),					AnimPath.IDLE_01,						0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle02),					AnimPath.IDLE_02,						0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouch01),				AnimPath.IDLE_SQUAT_01,					0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleCrouchShoot01),			AnimPath.IDLE_SQUAT_SHOOT_01,			0, AnimTag.NONE, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleShoot01),				AnimPath.IDLE_SHOOT_01,					0, AnimTag.NONE, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchForward01),		AnimPath.WALK_SQUAT_FORWARD_01,			0, AnimTag.WALK, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchBackward01),		AnimPath.WALK_SQUAT_BACKWARD_01,		0, AnimTag.WALK, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchLeft01),			AnimPath.WALK_SQUAT_LEFT_01,			0, AnimTag.WALK, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchRight01),			AnimPath.WALK_SQUAT_RIGHT_01,			0, AnimTag.WALK, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchForwardLeft01),	AnimPath.WALK_SQUAT_FORWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchForwardRight01),	AnimPath.WALK_SQUAT_FORWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchBackwardLeft01),	AnimPath.WALK_SQUAT_BACKWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkCrouchBackwardRight01),	AnimPath.WALK_SQUAT_BACKWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForward01),		AnimPath.WALK_SHOOT_FORWARD_01,			0, AnimTag.WALK, 30.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackward01),		AnimPath.WALK_SHOOT_BACKWARD_01,		0, AnimTag.WALK, 30.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootLeft01),			AnimPath.WALK_SHOOT_LEFT_01,			0, AnimTag.WALK, 30.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootRight01),			AnimPath.WALK_SHOOT_RIGHT_01,			0, AnimTag.WALK, 40.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForwardLeft01),	AnimPath.WALK_SHOOT_FORWARD_LEFT_01,	0, AnimTag.WALK, 30.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForwardRight01),	AnimPath.WALK_SHOOT_FORWARD_RIGHT_01,	0, AnimTag.WALK, 30.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackwardLeft01),	AnimPath.WALK_SHOOT_BACKWARD_LEFT_01,	0, AnimTag.WALK, 30.0f, true);
	AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackwardRight01),	AnimPath.WALK_SHOOT_BACKWARD_RIGHT_01,	0, AnimTag.WALK, 30.0f, true);

	AddAnimHandle(static_cast<int>(PlayerAnimKind::kRunForward01),				AnimPath.RUN_FORWARD_01,				0, AnimTag.WALK, 40.0f, true);
}

void PlayerAnimator::ChangeAnim()
{
	switch (m_state->GetCurrentMoveState()->GetStateKind())
	{
	case static_cast<int>(player_state::MoveStateKind::kMoveNull):
		AddToMoveNullState();
		break;

	case static_cast<int>(player_state::MoveStateKind::kMove):
		AddToMoveState();
		break;
	}
}

void PlayerAnimator::AddToMoveNullState()
{
	switch (m_state->GetCurrentActionState()->GetStateKind())
	{
	case static_cast<int>(player_state::ActionStateKind::kActionNull):
		AddToActionNullState();
		break;

	case static_cast<int>(player_state::ActionStateKind::kCrouch):
		AddToCrouchState();
		break;

	case static_cast<int>(player_state::ActionStateKind::kRun):
		AddToRunState();
		break;

	case static_cast<int>(player_state::ActionStateKind::kTurnAround):
		AddToTurnAround();
		break;
	}
}

void PlayerAnimator::AddToMoveState()
{
	switch (m_state->GetCurrentActionState()->GetStateKind())
	{
	case static_cast<int>(player_state::ActionStateKind::kActionNull):
		AddToActionNullState();
		break;

	case static_cast<int>(player_state::ActionStateKind::kCrouch):
		AddToCrouchState();
		break;

	case static_cast<int>(player_state::ActionStateKind::kRun):
		AddToRunState();
		break;

	case static_cast<int>(player_state::ActionStateKind::kTurnAround):
		AddToTurnAround();
		break;
	}
}

void PlayerAnimator::AddToActionNullState()
{
	switch (m_state->GetCurrentActionState()->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):
		AttachAnim(static_cast<int>(PlayerAnimKind::kIdle02));
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):
		
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kStabKnife):
		
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnife):
		
		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kParry):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAiming):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kShot):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):

		break;
	}
}

void PlayerAnimator::AddToCrouchState()
{
	switch (m_state->GetCurrentActionState()->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kStabKnife):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnife):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kParry):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAiming):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kShot):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):

		break;
	}
}

void PlayerAnimator::AddToRunState()
{
	switch (m_state->GetCurrentActionState()->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kStabKnife):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnife):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kParry):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAiming):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kShot):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):

		break;
	}
}

void PlayerAnimator::AddToTurnAround()
{
	switch (m_state->GetCurrentActionState()->GetStateKind())
	{
	case static_cast<int>(player_state::WeaponActionStateKind::kWeaponActionNull):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kKnifeEquipped):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kStabKnife):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnife):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kParry):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kGunEquipped):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kAiming):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kShot):

		break;

	case static_cast<int>(player_state::WeaponActionStateKind::kReload):

		break;
	}
}
