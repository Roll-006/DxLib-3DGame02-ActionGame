#include "zombie_state_controller.hpp"
#include "zombie_animator.hpp"

ZombieAnimator::ZombieAnimator(const std::shared_ptr<Modeler>& modeler,const std::shared_ptr<ZombieStateController>& state) :
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
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kIdle),							AnimPath.ZOMBIE_IDLE_01,				0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kMoveForwardWalk),				AnimPath.MOVE_FORWARD_WALK,				0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kMoveForwardRun),				AnimPath.MOVE_FORWARD_RUN_02,			0, AnimTag.MOVE, 60.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kEnterNeckBite),					AnimPath.ENTER_NECK_BITE,				0, AnimTag.NONE, 20.0f,  true, true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kEnterNeckBite),					AnimPath.EXIT_NECK_BITE,				0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kStealthKilled),					AnimPath.DEAD_03,						0, AnimTag.NONE, 60.0f,  false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kDead),							AnimPath.DEAD_01,						0, AnimTag.NONE, 60.0f,  false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kCrouchDead),					AnimPath.CROUCH_DEAD,					0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kFlyingKnockbackUp),				AnimPath.FLYING_KNOCKBACK_UP,			0, AnimTag.NONE, 35.0f,  false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kFlyingKnockbackDown),			AnimPath.FLYING_KNOCKBACK_DOWN,			0, AnimTag.NONE, 35.0f,	 false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kBackwardKnockback),				AnimPath.BACKWARD_KNOCKBACK,			0, AnimTag.NONE, 30.0f,	 false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kStandUp),						AnimPath.STAND_UP_ZOMBIE_01,			0, AnimTag.NONE, 35.0f,  false);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kStandUpkStandUpStealthKill),	AnimPath.STAND_UP_ZOMBIE_02,			0, AnimTag.NONE, 35.0f,  false);

	// 上半身用
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kGrab),							AnimPath.GRAB,							0, AnimTag.NONE, 10.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kStandStun),						AnimPath.STAND_STUN,					0, AnimTag.NONE, 2.0f,   true);

	// 下半身用
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kCrouchLeftStun),				AnimPath.LEANING_FORWARD_CROUCH_LEFT,	0, AnimTag.MOVE, 0.0f,   true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kCrouchRightStun),				AnimPath.LEANING_FORWARD_CROUCH_RIGHT,	0, AnimTag.MOVE, 0.0f,   true);
}

void ZombieAnimator::ChangeAnim()
{
	switch (static_cast<zombie_state::MoveStateKind>(m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind()))
	{
	case zombie_state::MoveStateKind::kMoveNull:
		CombineMoveNullWithAction();
		break;

	case zombie_state::MoveStateKind::kMove:
		CombineMoveWithAction();
		break;

	default:
		break;
	}
}


#pragma region 状態の合成
void ZombieAnimator::CombineMoveNullWithAction()
{
	const auto current_action_state_kind	= static_cast<zombie_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());
	const auto prev_action_state_kind		= static_cast<zombie_state::ActionStateKind>(m_state->GetActionState(TimeKind::kPrev)	->GetStateKind());

	switch (current_action_state_kind)
	{
	case zombie_state::ActionStateKind::kActionNull:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kIdle));
		break;

	case zombie_state::ActionStateKind::kStealthKilled:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kStealthKilled));
		break;

	case zombie_state::ActionStateKind::kGrab:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kEnterNeckBite));
		break;

	case zombie_state::ActionStateKind::kKnockback:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kFlyingKnockbackDown));
		break;

	case zombie_state::ActionStateKind::kBackwardKnockback:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kBackwardKnockback));
		break;

	case zombie_state::ActionStateKind::kStandUp:
		if (prev_action_state_kind == zombie_state::ActionStateKind::kStealthKilled)
		{
			AttachResultAnim(static_cast<int>(ZombieAnimKind::kStandUpkStandUpStealthKill));
		}
		else
		{
			AttachResultAnim(static_cast<int>(ZombieAnimKind::kStandUp));
		}
		break;

	case zombie_state::ActionStateKind::kStandStun:
		AttachAnim(static_cast<int>(ZombieAnimKind::kIdle),			BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(ZombieAnimKind::kStandStun),	BodyKind::kUpperBody);
		break;

	case zombie_state::ActionStateKind::kCrouchLeftStun:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kCrouchLeftStun));
		break;

	case zombie_state::ActionStateKind::kCrouchRightStun:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kCrouchRightStun));
		break;

	case zombie_state::ActionStateKind::kPlayDead:
		break;

	case zombie_state::ActionStateKind::kDead:
		if (	prev_action_state_kind != zombie_state::ActionStateKind::kKnockback
			 && prev_action_state_kind != zombie_state::ActionStateKind::kStealthKilled)
		{
			AttachResultAnim(static_cast<int>(ZombieAnimKind::kDead));
		}
		break;

	default:
		break;
	}
}

void ZombieAnimator::CombineMoveWithAction()
{
	switch (static_cast<zombie_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case zombie_state::ActionStateKind::kActionNull:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardWalk));
		break;

	case zombie_state::ActionStateKind::kRun:
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardRun));
		break;

	case zombie_state::ActionStateKind::kGrabRun:
		AttachAnim(static_cast<int>(ZombieAnimKind::kMoveForwardRun),	BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(ZombieAnimKind::kGrab),				BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}
#pragma endregion
