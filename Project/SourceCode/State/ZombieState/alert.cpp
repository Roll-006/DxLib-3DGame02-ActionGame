#include "alert.hpp"

zombie_state::Alert::Alert() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kActionNull))
{

}

zombie_state::Alert::~Alert()
{

}

void zombie_state::Alert::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Alert::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Alert::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Alert::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Alert::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
