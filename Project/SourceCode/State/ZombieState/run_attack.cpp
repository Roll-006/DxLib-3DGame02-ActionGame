#include "run_attack.hpp"

zombie_state::RunAttack::RunAttack() :
	AIStateBase(static_cast<int>(zombie_state::AIStateKind::kRunAttack))
{

}

zombie_state::RunAttack::~RunAttack()
{

}

void zombie_state::RunAttack::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::RunAttack::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::RunAttack::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::RunAttack::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::RunAttack::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
