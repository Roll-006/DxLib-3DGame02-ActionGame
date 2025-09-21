#include "alert.hpp"

zombie_state::Alert::Alert() :
	AIStateBase		(static_cast<int>(zombie_state::AIStateKind::kAlert)),
	m_is_all_stop	(false)
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
