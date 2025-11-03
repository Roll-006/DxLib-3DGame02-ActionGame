#include "boss_search.hpp"

boss_state::Search::Search() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kSearch)),
	m_is_stop_all_state	(false)
{

}

boss_state::Search::~Search()
{

}

void boss_state::Search::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Search::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Search::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Search::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Search::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ‹­§‘Ò‹@
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Boss>();
	}

	return nullptr;
}
