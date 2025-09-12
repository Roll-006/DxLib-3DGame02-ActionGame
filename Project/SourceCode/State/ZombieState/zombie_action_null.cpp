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
