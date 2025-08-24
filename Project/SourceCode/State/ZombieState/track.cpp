#include "track.hpp"

zombie_state::Track::Track() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kActionNull))
{

}

zombie_state::Track::~Track()
{

}

void zombie_state::Track::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Track::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Track::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Track::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Track::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
