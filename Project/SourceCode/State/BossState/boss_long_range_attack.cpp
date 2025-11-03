#include "boss_long_range_attack.hpp"

boss_state::LongRangeAttack::LongRangeAttack() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kLongRangeAttack)),
	m_is_stop_all_state	(false)
{

}

boss_state::LongRangeAttack::~LongRangeAttack()
{

}

void boss_state::LongRangeAttack::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::LongRangeAttack::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::LongRangeAttack::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::LongRangeAttack::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::LongRangeAttack::ChangeState(std::shared_ptr<Boss>& obj)
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
