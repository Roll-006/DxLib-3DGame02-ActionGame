#include "investigate.hpp"

zombie_state::investigate::investigate() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kActionNull))
{

}

zombie_state::investigate::~investigate()
{

}

void zombie_state::investigate::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::investigate::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::investigate::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::investigate::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::investigate::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
