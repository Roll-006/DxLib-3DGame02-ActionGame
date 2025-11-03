#include "boss_investigate.hpp"

boss_state::Investigate::Investigate() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kInvestigate)),
	m_is_stop_all_state	(false)
{

}

boss_state::Investigate::~Investigate()
{

}

void boss_state::Investigate::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Investigate::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Investigate::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Investigate::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Investigate::ChangeState(std::shared_ptr<Boss>& obj)
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
