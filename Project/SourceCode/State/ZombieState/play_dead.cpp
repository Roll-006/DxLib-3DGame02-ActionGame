#include "play_dead.hpp"

zombie_state::PlayDead::PlayDead() :
	ActionStateBase	(static_cast<int>(zombie_state::ActionStateKind::kPlayDead)),
	m_is_all_stop	(false)
{

}

zombie_state::PlayDead::~PlayDead()
{

}

void zombie_state::PlayDead::Update(std::shared_ptr<Zombie> obj)
{
	obj->CalcAttackIntervalTime();
}

void zombie_state::PlayDead::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::PlayDead::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::PlayDead::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::PlayDead::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// Ž€–S
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Zombie>();
	}

	return nullptr;
}
