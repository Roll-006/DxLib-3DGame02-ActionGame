#include "patrol.hpp"

zombie_state::Patrol::Patrol() :
	AIStateBase(static_cast<int>(zombie_state::AIStateKind::kPatrol))
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
	return nullptr;
}
