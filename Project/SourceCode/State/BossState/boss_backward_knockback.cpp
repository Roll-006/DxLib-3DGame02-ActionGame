#include "boss_backward_knockback.hpp"

boss_state::BackwardKnockback::BackwardKnockback() :
	ActionStateBase				(static_cast<int>(boss_state::ActionStateKind::kBackwardKnockback)),
	m_is_stop_all_state			(true)
{

}

boss_state::BackwardKnockback::~BackwardKnockback()
{

}

void boss_state::BackwardKnockback::Update(std::shared_ptr<Boss>& obj)
{
	obj->DisallowStealthKill();
}

void boss_state::BackwardKnockback::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::BackwardKnockback::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::BackwardKnockback::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::BackwardKnockback::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	if (obj->GetKnockBackSpeed() < math::kEpsilonLow)
	{
		// Ž€–S
		if (state_controller->TryDead(obj))
		{
			return state_controller->GetState<Dead, Boss>();
		}

		// NULL
		return state_controller->GetState<ActionNull, Boss>();
	}

	return nullptr;
}
