#include "search.hpp"

zombie_state::Search::Search() :
	ActionStateBase(static_cast<int>(zombie_state::ActionStateKind::kActionNull))
{

}

zombie_state::Search::~Search()
{

}

void zombie_state::Search::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Search::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Search::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Search::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Search::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
