#include "boss_knockback.hpp"

boss_state::Knockback::Knockback() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kKnockback)),
	m_is_stop_all_state	(true)
{

}

boss_state::Knockback::~Knockback()
{

}

void boss_state::Knockback::Update(std::shared_ptr<Boss>& obj)
{
	if (obj->CanAction())
	{
		obj->CalcAttackIntervalTime();
	}

	obj->DisallowStealthKill();
}

void boss_state::Knockback::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Knockback::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Knockback::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Knockback::ChangeState(std::shared_ptr<Boss>& obj)
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

		// —§‚¿ã‚ª‚é
		return state_controller->GetState<StandUp, Boss>();
	}

	return nullptr;
}
