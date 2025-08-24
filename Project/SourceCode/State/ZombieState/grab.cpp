#include "grab.hpp"

zombie_state::Grab::Grab() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kActionNull))
{

}

zombie_state::Grab::~Grab()
{

}

void zombie_state::Grab::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Grab::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Grab::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Grab::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Grab::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
