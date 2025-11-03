#include "boss_track.hpp"

boss_state::Track::Track() :
	AIStateBase			(static_cast<int>(boss_state::AIStateKind::kTrack)),
	m_is_stop_all_state	(false)
{

}

boss_state::Track::~Track()
{

}

void boss_state::Track::Update(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Track::LateUpdate(std::shared_ptr<Boss>& obj)
{

}

void boss_state::Track::Enter(std::shared_ptr<Boss>& obj)
{
	
}

void boss_state::Track::Exit(std::shared_ptr<Boss>& obj)
{

}

std::shared_ptr<IState<Boss>> boss_state::Track::ChangeState(std::shared_ptr<Boss>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制待機
	if (state_controller->TryWaitForcibly(obj))
	{
		return state_controller->GetState<Wait, Boss>();
	}
	// ダッシュ攻撃
	if (state_controller->TryRunAttack(obj))
	{
		return state_controller->GetState<RunAttack, Boss>();
	}
	// 待機
	if (!state_controller->TryTrack(obj) && !state_controller->TryGrabRun())
	{
		return state_controller->GetState<Wait, Boss>();
	}

	return nullptr;
}
