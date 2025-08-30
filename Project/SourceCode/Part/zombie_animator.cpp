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
	// 上半身
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kIdle),				AnimPath.ZOMBIE_IDLE_01,		1, AnimTag.NONE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kMoveForwardWalk),	AnimPath.MOVE_FORWARD_WALK,		1, AnimTag.MOVE, 20.0f,  true);
	AddAnimHandle(static_cast<int>(ZombieAnimKind::kMoveForwardRun),	AnimPath.MOVE_FORWARD_RUN_02,	1, AnimTag.MOVE, 20.0f,  true);
}

void ZombieAnimator::ChangeAnim()
{
	switch (m_state->GetActionState(TimeKind::kCurrent)->GetStateKind())
	{
	case static_cast<int>(zombie_state::ActionStateKind::kActionNull):
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kIdle));
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kWalk):
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardWalk));
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kRun):
		AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardRun));
		break;

	case static_cast<int>(zombie_state::ActionStateKind::kGrab):
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


#pragma region 状態の合成

#pragma endregion
