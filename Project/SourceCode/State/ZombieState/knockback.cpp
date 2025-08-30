#include "knockback.hpp"

zombie_state::Knockback::Knockback() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kKnockback))
{

}

zombie_state::Knockback::~Knockback()
{

}

void zombie_state::Knockback::Update(std::shared_ptr<Zombie> obj)
{
	
}

void zombie_state::Knockback::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Knockback::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Knockback::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Knockback::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
