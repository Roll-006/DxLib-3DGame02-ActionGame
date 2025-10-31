#include "grab_run.hpp"

zombie_state::GrabRun::GrabRun() :
	ActionStateBase		(static_cast<int>(zombie_state::ActionStateKind::kGrabRun)),
	m_track_timer		(0.0f),
	m_is_stop_all_state	(false)
{

}

zombie_state::GrabRun::~GrabRun()
{

}

void zombie_state::GrabRun::Update(std::shared_ptr<Zombie>& obj)
{
	m_track_timer += obj->GetDeltaTime();

	obj->UpdateGrabRun();
}

void zombie_state::GrabRun::LateUpdate(std::shared_ptr<Zombie>& obj)
{

}

void zombie_state::GrabRun::Enter(std::shared_ptr<Zombie>& obj)
{
	m_track_timer = 0.0f;

	obj->SetAttackIntervalTime();
}

void zombie_state::GrabRun::Exit(std::shared_ptr<Zombie>& obj)
{
	
}

std::shared_ptr<IState<Zombie>> zombie_state::GrabRun::ChangeState(std::shared_ptr<Zombie>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 強制NULL
	if (state_controller->TryActionNullForcibly(obj))
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}
	// ステルスキルされた
	if (state_controller->TryStealthKilled(obj))
	{
		return state_controller->GetState<StealthKilled, Zombie>();
	}
	// ノックバック
	if (state_controller->TryKnockback(obj))
	{
		return state_controller->GetState<Knockback, Zombie>();
	}
	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Zombie>();
	}
	// 左足ダウン
	if (state_controller->TryLeftCrouchStun(obj))
	{
		return state_controller->GetState<CrouchLeftStun, Zombie>();
	}
	// 右足ダウン
	if (state_controller->TryRightCrouchStun(obj))
	{
		return state_controller->GetState<CrouchRightStun, Zombie>();
	}
	// 立ちダウン
	if (state_controller->TryStandStun(obj))
	{
		return state_controller->GetState<StandStun, Zombie>();
	}
	// 掴み
	if (obj->CanGrabTarget())
	{
		return state_controller->GetState<Grab, Zombie>();
	}
	// NULL
	if (m_track_timer > kMaxTrackTime)
	{
		return state_controller->GetState<ActionNull, Zombie>();
	}

	return nullptr;
}
