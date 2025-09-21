#include "investigate.hpp"

zombie_state::Investigate::Investigate() :
	AIStateBase		(static_cast<int>(zombie_state::AIStateKind::kInvestigate)),
	m_is_all_stop	(false)
{

}

zombie_state::Investigate::~Investigate()
{

}

void zombie_state::Investigate::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Investigate::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Investigate::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Investigate::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Investigate::ChangeState(std::shared_ptr<Zombie> obj)
{
	return nullptr;
}
