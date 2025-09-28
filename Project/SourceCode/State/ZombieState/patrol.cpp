#include "patrol.hpp"

zombie_state::Patrol::Patrol() :
	AIStateBase		(static_cast<int>(zombie_state::AIStateKind::kPatrol)),
	m_is_all_stop	(false)
{

}

zombie_state::Patrol::~Patrol()
{

}

void zombie_state::Patrol::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Patrol::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Patrol::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Patrol::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Patrol::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// ‹­§‘Ò‹@
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Zombie>();
	}

	return nullptr;
}
