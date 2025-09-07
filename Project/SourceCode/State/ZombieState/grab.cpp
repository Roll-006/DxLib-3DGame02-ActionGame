#include "grab.hpp"

zombie_state::Grab::Grab() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kGrab))
{

}

zombie_state::Grab::~Grab()
{

}

void zombie_state::Grab::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Grab::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Grab::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Grab::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Grab::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
