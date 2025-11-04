#include "../Part/boss_state_controller.hpp"
#include "boss_animator.hpp"

BossAnimator::BossAnimator(const std::shared_ptr<Modeler>& modeler,const std::shared_ptr<BossStateController>& state) :
	AnimatorBase	(modeler, ObjName.BOSS),
	m_state			(state)
{
	LoadAnim();
}

BossAnimator::~BossAnimator()
{
	
}

void BossAnimator::Init()
{

}

void BossAnimator::Update()
{
	DivideBone();
	ChangeAnim();

	BlendAnim();
	PlayAnim();
}

void BossAnimator::LoadAnim()
{
	// 汎用
	AddAnimHandle(static_cast<int>(BossAnimKind::kIdle),						BossAnimPath.ZOMBIE_IDLE_01,				0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kDetected),					BossAnimPath.SHOUT_ZOMBIE,					0, AnimTag.NONE, 50.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kMoveForwardWalk),				BossAnimPath.MOVE_FORWARD_WALK,				0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kMoveForwardRun),				BossAnimPath.MOVE_FORWARD_RUN_02,			0, AnimTag.MOVE, 60.0f,  true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kEnterNeckBite),				BossAnimPath.ENTER_NECK_BITE,				0, AnimTag.NONE, 20.0f,  true, true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kEnterNeckBite),				BossAnimPath.EXIT_NECK_BITE,				0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kStealthKilled),				BossAnimPath.DEAD_03,						0, AnimTag.NONE, 60.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kDead),						BossAnimPath.DEAD,						0, AnimTag.NONE, 60.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kCrouchDead),					BossAnimPath.CROUCH_DEAD,					0, AnimTag.NONE, 20.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kFlyingKnockbackUp),			BossAnimPath.FLYING_KNOCKBACK_UP,			0, AnimTag.NONE, 35.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kFlyingKnockbackDown),			BossAnimPath.FLYING_KNOCKBACK_DOWN,			0, AnimTag.NONE, 35.0f,	 false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kBackwardKnockback),			BossAnimPath.BACKWARD_KNOCKBACK,			0, AnimTag.NONE, 30.0f,	 false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kStandUp),						BossAnimPath.STAND_UP_ZOMBIE_01,			0, AnimTag.NONE, 35.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kStandUpkStandUpStealthKill),	BossAnimPath.STAND_UP_ZOMBIE_02,			0, AnimTag.NONE, 35.0f,  false);

	// 上半身用
	AddAnimHandle(static_cast<int>(BossAnimKind::kGrab),						BossAnimPath.GRAB,							0, AnimTag.NONE, 10.0f,  true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kStandStun),					BossAnimPath.STAND_STUN,					0, AnimTag.NONE, 2.0f,   true);

	// 下半身用
	AddAnimHandle(static_cast<int>(BossAnimKind::kCrouchLeftStun),				BossAnimPath.LEANING_FORWARD_CROUCH_LEFT,	0, AnimTag.MOVE, 0.0f,   true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kCrouchRightStun),				BossAnimPath.LEANING_FORWARD_CROUCH_RIGHT,	0, AnimTag.MOVE, 0.0f,   true);
}

void BossAnimator::ChangeAnim()
{
	switch (static_cast<boss_state::MoveStateKind>(m_state->GetMoveState(TimeKind::kCurrent)->GetStateKind()))
	{
	case boss_state::MoveStateKind::kMoveNull:
		CombineMoveNullWithAction();
		break;

	case boss_state::MoveStateKind::kMove:
		CombineMoveWithAction();
		break;

	default:
		break;
	}
}


#pragma region 状態の合成
void BossAnimator::CombineMoveNullWithAction()
{
	const auto current_action_state_kind	= static_cast<boss_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());
	const auto prev_action_state_kind		= static_cast<boss_state::ActionStateKind>(m_state->GetActionState(TimeKind::kPrev)	->GetStateKind());

	switch (current_action_state_kind)
	{
	case boss_state::ActionStateKind::kActionNull:
		AttachResultAnim(static_cast<int>(BossAnimKind::kIdle));
		break;

	case boss_state::ActionStateKind::kDetected:
		AttachResultAnim(static_cast<int>(BossAnimKind::kDetected));
		break;

	case boss_state::ActionStateKind::kStealthKilled:
		AttachResultAnim(static_cast<int>(BossAnimKind::kStealthKilled));
		break;

	case boss_state::ActionStateKind::kGrab:
		AttachResultAnim(static_cast<int>(BossAnimKind::kEnterNeckBite));
		break;

	case boss_state::ActionStateKind::kKnockback:
		AttachResultAnim(static_cast<int>(BossAnimKind::kFlyingKnockbackDown));
		break;

	case boss_state::ActionStateKind::kBackwardKnockback:
		AttachResultAnim(static_cast<int>(BossAnimKind::kBackwardKnockback));
		break;

	case boss_state::ActionStateKind::kStandUp:
		if (prev_action_state_kind == boss_state::ActionStateKind::kStealthKilled)
		{
			AttachResultAnim(static_cast<int>(BossAnimKind::kStandUpkStandUpStealthKill));
		}
		else
		{
			AttachResultAnim(static_cast<int>(BossAnimKind::kStandUp));
		}
		break;

	case boss_state::ActionStateKind::kStandStun:
		AttachAnim(static_cast<int>(BossAnimKind::kIdle),			BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(BossAnimKind::kStandStun),	BodyKind::kUpperBody);
		break;

	case boss_state::ActionStateKind::kCrouchLeftStun:
		AttachResultAnim(static_cast<int>(BossAnimKind::kCrouchLeftStun));
		break;

	case boss_state::ActionStateKind::kCrouchRightStun:
		AttachResultAnim(static_cast<int>(BossAnimKind::kCrouchRightStun));
		break;

	case boss_state::ActionStateKind::kPlayDead:
		break;

	case boss_state::ActionStateKind::kDead:
		AttachResultAnim(static_cast<int>(BossAnimKind::kDead));
		break;

	default:
		break;
	}
}

void BossAnimator::CombineMoveWithAction()
{
	switch (static_cast<boss_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind()))
	{
	case boss_state::ActionStateKind::kActionNull:
		AttachResultAnim(static_cast<int>(BossAnimKind::kMoveForwardWalk));
		break;

	case boss_state::ActionStateKind::kRun:
		AttachResultAnim(static_cast<int>(BossAnimKind::kMoveForwardRun));
		break;

	case boss_state::ActionStateKind::kGrabRun:
		AttachAnim(static_cast<int>(BossAnimKind::kMoveForwardRun),	BodyKind::kLowerBody);
		AttachAnim(static_cast<int>(BossAnimKind::kGrab),				BodyKind::kUpperBody);
		break;

	default:
		break;
	}
}
#pragma endregion
