#include "grab_run.hpp"

zombie_state::GrabRun::GrabRun() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kGrabRun))
{

}

zombie_state::GrabRun::~GrabRun()
{

}

void zombie_state::GrabRun::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::GrabRun::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::GrabRun::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::GrabRun::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::GrabRun::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
