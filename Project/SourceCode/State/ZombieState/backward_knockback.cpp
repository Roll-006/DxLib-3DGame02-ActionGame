#include "backward_knockback.hpp"

zombie_state::BackwardKnockback::BackwardKnockback() :
	ActionStateBase				(static_cast<int>(zombie_state::ActionStateKind::kBackwardKnockback)),
	m_is_stop_all_state			(true)
{

}

zombie_state::BackwardKnockback::~BackwardKnockback()
{

}

void zombie_state::BackwardKnockback::Update(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::BackwardKnockback::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::BackwardKnockback::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::BackwardKnockback::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::BackwardKnockback::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	if (obj->GetKnockBackSpeed() < math::kEpsilonLow)
	{
		// Ž€–S
		if (state_controller->TryDead(obj))
		{
			return state_controller->GetState<Dead, Zombie>();
		}

		// NULL
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
