#include "knockback.hpp"

zombie_state::Knockback::Knockback() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kKnockback)),
	m_is_stop_all_state	(true)
{

}

zombie_state::Knockback::~Knockback()
{

}

void zombie_state::Knockback::Update(std::shared_ptr<Zombie>& obj)
{
	if (obj->CanAction())
	{
		obj->CalcAttackIntervalTime();
	}

	obj->DisallowStealthKill();
	obj->DisallowDecreaseKnockBackGauge();
}

void zombie_state::Knockback::LateUpdate(std::shared_ptr<Zombie>& obj)
{
	obj->DisallowProjectPos();
}

void zombie_state::Knockback::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Knockback::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Knockback::ChangeState(std::shared_ptr<Zombie>& obj)
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

		// —§‚¿ã‚ª‚é
		return state_controller->GetState<StandUp, Zombie>();
	}

	return nullptr;
}
