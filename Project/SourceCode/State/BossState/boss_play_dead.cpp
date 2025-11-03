#include "boss_play_dead.hpp"

boss_state::PlayDead::PlayDead() :
	ActionStateBase		(static_cast<int>(boss_state::ActionStateKind::kPlayDead)),
	m_is_stop_all_state	(false)
{

}

boss_state::PlayDead::~PlayDead()
{

}

void boss_state::PlayDead::Update(std::shared_ptr<Boss>& obj)
{
	if (obj->CanAction())
	{
		obj->CalcAttackIntervalTime();
	}
}

void boss_state::PlayDead::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::PlayDead::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::PlayDead::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::PlayDead::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// Ž€–S
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Boss>();
	}

	return nullptr;
}
