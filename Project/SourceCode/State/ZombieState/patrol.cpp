#include "patrol.hpp"

zombie_state::Patrol::Patrol() :
	AIStateBase			(static_cast<int>(zombie_state::AIStateKind::kPatrol)),
	m_is_stop_all_state	(false)
{

}

zombie_state::Patrol::~Patrol()
{

}

void zombie_state::Patrol::Update(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Patrol::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Patrol::Enter(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::Patrol::Exit(std::shared_ptr<Zombie>& obj)
{

}

std::shared_ptr<IState<Zombie>> zombie_state::Patrol::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制待機
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Zombie>();
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
