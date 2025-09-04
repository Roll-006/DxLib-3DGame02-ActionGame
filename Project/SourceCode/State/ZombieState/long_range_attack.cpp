#include "long_range_attack.hpp"

zombie_state::LongRangeAttack::LongRangeAttack() :
	AIStateBase(static_cast<int>(zombie_state::AIStateKind::kLongRangeAttack))
{

}

zombie_state::LongRangeAttack::~LongRangeAttack()
{

}

void zombie_state::LongRangeAttack::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::LongRangeAttack::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::LongRangeAttack::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::LongRangeAttack::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::LongRangeAttack::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
