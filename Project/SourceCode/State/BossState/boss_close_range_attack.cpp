#include "boss_close_range_attack.hpp"

boss_state::CloseRangeAttack::CloseRangeAttack() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kCloseRangeAttack)),
	m_is_stop_all_state	(false)
{

}

boss_state::CloseRangeAttack::~CloseRangeAttack()
{

}

void boss_state::CloseRangeAttack::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::CloseRangeAttack::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::CloseRangeAttack::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::CloseRangeAttack::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::CloseRangeAttack::ChangeState(std::shared_ptr<Boss>& obj)
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
