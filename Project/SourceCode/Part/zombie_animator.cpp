#include "zombie_state_controller.hpp"
#include "zombie_animator.hpp"

ZombieAnimator::ZombieAnimator(const std::shared_ptr<Modeler> modeler,const std::shared_ptr<ZombieStateController> state) :
	AnimatorBase	(modeler, ObjName.ZOMBIE),
	m_state			(state)
{
	LoadAnim();
}

ZombieAnimator::~ZombieAnimator()
{
	
}

void ZombieAnimator::Init()
{

}

void ZombieAnimator::Update()
{
	DivideBone();
	ChangeAnim();

	BlendAnim();
	PlayAnim();
}

void ZombieAnimator::LoadAnim()
{
	// 汎用
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kIdle),				AnimPath.ZOMBIE_IDLE_01,				0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kMoveForwardWalk),	AnimPath.MOVE_FORWARD_WALK,				0, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kMoveForwardRun),	AnimPath.MOVE_FORWARD_RUN_02,			0, AnimTag.MOVE, 20.0f,  true);

	// 上半身用
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kGrab),				AnimPath.GRAB,							1, AnimTag.NONE, 10.0f,  true);

	// 下半身用
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kCrouchStunLeft),	AnimPath.LEANING_FORWARD_CROUCH_LEFT,	0, AnimTag.MOVE, 0.0f,   true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kCrouchStunRight),	AnimPath.LEANING_FORWARD_CROUCH_RIGHT,	0, AnimTag.MOVE, 0.0f,   true);
}

void ZombieAnimator::ChangeAnim()
{
	switch (m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(zombie_state::MoveStateKind::kMoveNull):
		CombineMoveNullWithAction();
		break;

	case static_cast<int>(zombie_state::MoveStateKind::kMove):
		CombineMoveWithAction();
		break;

	default:
		break;
	}
}


#pragma region 状態の合成
void ZombieAnimator::CombineMoveNullWithAction()
{
	switch (m_state->GetActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(zombie_state::ActionStateKind::kActionNull):
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kIdle));
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kGrab):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kKnockback):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kStandStun):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kCrouchStun):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kPlayDead):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kDead):
		break;

	default:
		break;
	}
}

void ZombieAnimator::CombineMoveWithAction()
{
	switch (m_state->GetActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(zombie_state::ActionStateKind::kActionNull):
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardWalk));
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kRun):
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardRun));
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kGrabRun):
		AttachAnim(static_cast<int>(ZombieAnimKind::kMoveForwardRun),	BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(ZombieAnimKind::kGrab),				BodyKind::kUpperBody);
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kKnockback):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kStandStun):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kCrouchStun):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kPlayDead):
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kDead):
		break;

	default:
		break;
	}
}
#pragma endregion
