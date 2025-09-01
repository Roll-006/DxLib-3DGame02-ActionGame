#include "zombie_run.hpp"

zombie_state::Run::Run() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kRun))
{

}

zombie_state::Run::~Run()
{

}

void zombie_state::Run::Update(std::shared_ptr<Zombie> obj)
{
	
}

void zombie_state::Run::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Run::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Run::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Run::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
