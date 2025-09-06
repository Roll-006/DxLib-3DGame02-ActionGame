#include "wait.hpp"

zombie_state::Wait::Wait() : 
	AIStateBase(static_cast<int>(zombie_state::AIStateKind::kWait))
{

}

zombie_state::Wait::~Wait()
{

}

void zombie_state::Wait::Update(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Wait::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Wait::Enter(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::Wait::Exit(std::shared_ptr<Zombie> obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Wait::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// TODO : ‰¼‚Å‚Â‚©‚İˆ—‚ğ‹­§BŒã‚ÉíœB
	if (state_controller->TryTrack(obj))
	{
		return state_controller->GetState<RunAttack, Zombie>();
	}
	// ’ÇÕ
	if (state_controller->TryTrack(obj))
	{
		return state_controller->GetState<Track, Zombie>();
	}

	return nullptr;
}
