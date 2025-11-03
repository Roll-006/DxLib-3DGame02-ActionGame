#include "boss_stand_up.hpp"

boss_state::StandUp::StandUp() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kStandUp)),
	m_is_stop_all_state	(true)
{

}

boss_state::StandUp::~StandUp()
{

}

void boss_state::StandUp::Update(std::shared_ptr<Boss>& obj)
{
	if (obj->CanAction())
	{
		obj->CalcAttackIntervalTime();
	}

	obj->DisallowStealthKill();
}

void boss_state::StandUp::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::StandUp::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::StandUp::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::StandUp::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ステルスキルされた
	if (state_controller->TryStealthKilled(obj))
	{
		return state_controller->GetState<StealthKilled, Boss>();
	}
	// ノックバック
	if (state_controller->TryKnockback(obj))
	{
		return state_controller->GetState<Knockback, Boss>();
	}
	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Boss>();
	}

	return nullptr;
}
