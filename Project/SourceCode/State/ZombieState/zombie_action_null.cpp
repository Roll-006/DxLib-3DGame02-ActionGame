#include "zombie_action_null.hpp"

zombie_state::ActionNull::ActionNull() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kActionNull))
{

}

zombie_state::ActionNull::~ActionNull()
{

}

void zombie_state::ActionNull::Update(std::shared_ptr<Zombie> obj)
{
	obj->CalcAttackIntervalTime();
}

void zombie_state::ActionNull::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::ActionNull::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::ActionNull::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::ActionNull::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Zombie>();
	}
	// 左足ダウン
	if (state_controller->TryLeftCrouchStun(obj))
	{
		return state_controller->GetState<CrouchLeftStun, Zombie>();
	}
	// 右足ダウン
	if (state_controller->TryRightCrouchStun(obj))
	{
		return state_controller->GetState<CrouchRightStun, Zombie>();
	}
	// 立ちダウン
	if (state_controller->TryStandStun(obj))
	{
		return state_controller->GetState<StandStun, Zombie>();
	}
	// ダッシュ掴み
	if (state_controller->TryGrabRun())
	{
		return state_controller->GetState<GrabRun, Zombie>();
	}
	// ダッシュ
	if (state_controller->TryRun(obj))
	{
		return state_controller->GetState<Run, Zombie>();
	}

	return nullptr;
}
