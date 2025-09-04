#include "close_range_attack.hpp"

zombie_state::CloseRangeAttack::CloseRangeAttack() :
	AIStateBase(static_cast<int>(zombie_state::AIStateKind::kCloseRangeAttack))
{

}

zombie_state::CloseRangeAttack::~CloseRangeAttack()
{

}

void zombie_state::CloseRangeAttack::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CloseRangeAttack::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CloseRangeAttack::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::CloseRangeAttack::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::CloseRangeAttack::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
