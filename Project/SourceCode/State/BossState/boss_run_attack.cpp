#include "boss_run_attack.hpp"

boss_state::RunAttack::RunAttack() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kRunAttack)),
	m_is_stop_all_state	(false)
{

}

boss_state::RunAttack::~RunAttack()
{

}

void boss_state::RunAttack::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::RunAttack::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::RunAttack::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::RunAttack::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::RunAttack::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ‹­§‘Ò‹@
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Boss>();
	}
	// ‘Ò‚¿
	if (!state_controller->TryRunAttack(obj))
	{
		return state_controller->GetState<Wait, Boss>();
	}

	return nullptr;
}
