#include "grab_run.hpp"

zombie_state::GrabRun::GrabRun() :
	ActionStateBase	(static_cast<int>(zombie_state::ActionStateKind::kGrabRun)),
	m_track_timer	(0.0f)
{

}

zombie_state::GrabRun::~GrabRun()
{

}

void zombie_state::GrabRun::Update(std::shared_ptr<Zombie> obj)
{
	m_track_timer += obj->GetDeltaTime();

	obj->UpdateGrabRun();
}

void zombie_state::GrabRun::LateUpdate(std::shared_ptr<Zombie> obj)
{

}

void zombie_state::GrabRun::Enter(std::shared_ptr<Zombie> obj)
{
	m_track_timer = 0.0f;

	obj->SetAttackIntervalTime();
}

void zombie_state::GrabRun::Exit(std::shared_ptr<Zombie> obj)
{
	
}

std::shared_ptr<IState<Zombie>> zombie_state::GrabRun::ChangeState(std::shared_ptr<Zombie> obj)
{
	const auto state_controller = obj->GetStateController();

	// ’Í‚Ý
	if (obj->CanGrabTraget())
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
