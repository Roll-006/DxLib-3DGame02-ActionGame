#include "boss_wait.hpp"

boss_state::Wait::Wait() : 
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kWait)),
	m_is_stop_all_state	(false)
{

}

boss_state::Wait::~Wait()
{

}

void boss_state::Wait::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Wait::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Wait::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Wait::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Wait::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制待機
	if (state_controller->TryWaitForcibly(obj))
	{
		return nullptr;
	}
	// 巡回
	if (state_controller->TryPatrol(obj))
	{
		return state_controller->GetState<Patrol, Boss>();
	}
	// ダッシュ攻撃
	if (state_controller->TryRunAttack(obj))
	{
		return state_controller->GetState<RunAttack, Boss>();
	}
	// 追跡
	if (state_controller->TryTrack(obj))
	{
		return state_controller->GetState<Track, Boss>();
	}

	return nullptr;
}
