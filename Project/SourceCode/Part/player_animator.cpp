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
	
}
