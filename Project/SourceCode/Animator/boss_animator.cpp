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
	AddAnimHandle(static_cast<int>(BossAnimKind::kIdle),						BossAnimPath.IDLE,							0, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kDetected),					BossAnimPath.SHOUT,							0, AnimTag.NONE, 50.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kMoveForwardWalk),				BossAnimPath.MOVE_FORWARD_WALK,				0, AnimTag.MOVE, 55.0f,  true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kMoveForwardRun),				BossAnimPath.MOVE_FORWARD_RUN,				0, AnimTag.MOVE, 60.0f,  true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kStealthKilled),				BossAnimPath.DEAD_01,						0, AnimTag.NONE, 60.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kDead),						BossAnimPath.DEAD_02,						0, AnimTag.NONE, 60.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kFlyingKnockback),				BossAnimPath.FLYING_KNOCKBACK,				0, AnimTag.NONE, 35.0f,	 false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kBackwardKnockback),			BossAnimPath.BACKWARD_KNOCKBACK,			0, AnimTag.NONE, 30.0f,	 false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kStandUp),						BossAnimPath.STAND_UP_01,					0, AnimTag.NONE, 35.0f,  false);
	AddAnimHandle(static_cast<int>(BossAnimKind::kStandUpkStandUpStealthKill),	BossAnimPath.STAND_UP_02,					0, AnimTag.NONE, 35.0f,  false);

	// 上半身用
	AddAnimHandle(static_cast<int>(BossAnimKind::kStandStun),					BossAnimPath.STAND_STUN,					0, AnimTag.NONE, 1.0f,   true);

	// 下半身用
	AddAnimHandle(static_cast<int>(BossAnimKind::kCrouchLeftStun),				BossAnimPath.LEANING_FORWARD_CROUCH_LEFT,	0, AnimTag.MOVE, 40.0f,  true, true);
	AddAnimHandle(static_cast<int>(BossAnimKind::kCrouchRightStun),				BossAnimPath.LEANING_FORWARD_CROUCH_RIGHT,	0, AnimTag.MOVE, 40.0f,  true, true);
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

	case boss_state::ActionStateKind::kKnockback:
		AttachResultAnim(static_cast<int>(BossAnimKind::kFlyingKnockback));
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

	default:
		break;
	}
}
#pragma endregion
