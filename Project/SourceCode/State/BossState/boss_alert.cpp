#include "boss_alert.hpp"

boss_state::Alert::Alert() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kAlert)),
	m_is_stop_all_state	(false)
{

}

boss_state::Alert::~Alert()
{

}

void boss_state::Alert::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Alert::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Alert::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Alert::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Alert::ChangeState(std::shared_ptr<Boss>& obj)
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
