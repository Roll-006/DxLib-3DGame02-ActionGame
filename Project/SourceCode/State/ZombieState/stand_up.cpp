#include "stand_up.hpp"

zombie_state::StandUp::StandUp() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kStandUp)),
	m_is_stop_all_state	(true)
{

}

zombie_state::StandUp::~StandUp()
{

}

void zombie_state::StandUp::Update(std::shared_ptr<Zombie>& obj)
{
	if (obj->CanAction())
	{
		obj->CalcAttackIntervalTime();
	}

	obj->DisallowStealthKill();
}

void zombie_state::StandUp::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::StandUp::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::StandUp::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::StandUp::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ステルスキルされた
	if (state_controller->TryStealthKilled(obj))
	{
		return state_controller->GetState<StealthKilled, Zombie>();
	}
	// ノックバック
	if (state_controller->TryKnockback(obj))
	{
		return state_controller->GetState<Knockback, Zombie>();
	}
	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
