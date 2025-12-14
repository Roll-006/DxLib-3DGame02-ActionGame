#include "grabbed.hpp"

player_state::Grabbed::Grabbed(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kGrabbed)),
	m_is_stop_all_state	(true),
	m_player			(player)
{

}

player_state::Grabbed::~Grabbed()
{

}

void player_state::Grabbed::Update()
{
	obj->UpdateGrabbed();
}

void player_state::Grabbed::LateUpdate()
{
	obj->OnFootIK();
}

void player_state::Grabbed::Enter()
{
	// 掴まれたことを演出カメラに通知
	const OnGrabEvent event{ obj->GetModeler(), obj->GetObjHandle() };
	EventSystem::GetInstance()->Publish(event);
}

void player_state::Grabbed::Exit()
{

}

int player_state::Grabbed::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 死亡
	if (state_controller->TryDead(obj))
	{
		return state_controller->GetState<Dead, Player>();
	}
	// 脱出
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
