#include "close_range_attack.hpp"

zombie_state::CloseRangeAttack::CloseRangeAttack() :
	AIStateBase			(static_cast<int>(zombie_state::AIStateKind::kCloseRangeAttack)),
	m_is_stop_all_state	(false)
{

}

zombie_state::CloseRangeAttack::~CloseRangeAttack()
{

}

void zombie_state::CloseRangeAttack::Update(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::CloseRangeAttack::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::CloseRangeAttack::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::CloseRangeAttack::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::CloseRangeAttack::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ‹­§‘Ò‹@
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Zombie>();
	}

	return nullptr;
}
