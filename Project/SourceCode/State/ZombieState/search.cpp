#include "search.hpp"

zombie_state::Search::Search() :
	AIStateBase		(static_cast<int>(zombie_state::AIStateKind::kSearch)),
	m_is_all_stop	(false)
{

}

zombie_state::Search::~Search()
{

}

void zombie_state::Search::Update(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Search::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Search::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Search::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Search::ChangeState(std::shared_ptr<Zombie>& obj)
{
	const auto state_controller = obj->GetStateController();

	// ‹­§‘Ò‹@
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Zombie>();
	}

	return nullptr;
}
