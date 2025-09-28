#include "long_range_attack.hpp"

zombie_state::LongRangeAttack::LongRangeAttack() :
	AIStateBase		(static_cast<int>(zombie_state::AIStateKind::kLongRangeAttack)),
	m_is_all_stop	(false)
{

}

zombie_state::LongRangeAttack::~LongRangeAttack()
{

}

void zombie_state::LongRangeAttack::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::LongRangeAttack::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::LongRangeAttack::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::LongRangeAttack::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::LongRangeAttack::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// ‹­§‘Ò‹@
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Zombie>();
	}

	return nullptr;
}
