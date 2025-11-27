#include "grabbed.hpp"

player_state::Grabbed::Grabbed() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kGrabbed)),
	m_is_stop_all_state	(true)
{

}

player_state::Grabbed::~Grabbed()
{

}

void player_state::Grabbed::Update(std::shared_ptr<Player>& obj)
{
	obj->UpdateGrabbed();
}

void player_state::Grabbed::LateUpdate(std::shared_ptr<Player>& obj)
{
	obj->OnFootIK();
}

void player_state::Grabbed::Enter(std::shared_ptr<Player>& obj)
{
	// ’Í‚Ü‚ê‚½‚±‚Æ‚ð‰‰oƒJƒƒ‰‚É’Ê’m
	const OnGrabEvent event{ obj->GetModeler(), obj->GetObjHandle() };
	EventSystem::GetInstance()->Publish(event);
}

void player_state::Grabbed::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::Grabbed::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// Ž€–S
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Player>();
	}
	// ’Eo
	if (obj->IsEscape())
	{
		return state_controller->GetState<Escape, Player>();
	}
	// NULL
	if (!state_controller->TryGrabbed(obj))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
