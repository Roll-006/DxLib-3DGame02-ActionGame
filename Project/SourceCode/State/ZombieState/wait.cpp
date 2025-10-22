#include "wait.hpp"

zombie_state::Wait::Wait() : 
	AIStateBase			(static_cast<int>(zombie_state::AIStateKind::kWait)),
	m_is_stop_all_state	(false)
{

}

zombie_state::Wait::~Wait()
{

}

void zombie_state::Wait::Update(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Wait::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Wait::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Wait::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Wait::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制待機
	if (state_controller->TryWaitForcibly(obj))
	{
		return nullptr;
	}
	// ダッシュ攻撃
	if (state_controller->TryRunAttack(obj))
	{
		return state_controller->GetState<RunAttack, Zombie>();
	}
	// 追跡
	if (state_controller->TryTrack(obj))
	{
		return state_controller->GetState<Track, Zombie>();
	}

	return nullptr;
}
