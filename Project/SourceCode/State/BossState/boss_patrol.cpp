#include "boss_patrol.hpp"

boss_state::Patrol::Patrol() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kPatrol)),
	m_is_stop_all_state	(false)
{

}

boss_state::Patrol::~Patrol()
{

}

void boss_state::Patrol::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Patrol::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Patrol::Enter(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Patrol::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Patrol::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制待機
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Boss>();
	}
	// 待機
	if (obj->GetPatrolRouteGiver()->IsEnd())
	{
		return state_controller->GetState<Wait, Boss>();
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
