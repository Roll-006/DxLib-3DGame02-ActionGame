#include "battle.hpp"

zombie_state::Battle::Battle() :
	AIStateBase(static_cast<int>(zombie_state::AIStateKind::kBattle))
{

}

zombie_state::Battle::~Battle()
{

}

void zombie_state::Battle::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Battle::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Battle::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Battle::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Battle::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
