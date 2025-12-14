#include "finish_off.hpp"

player_state::FinishOff::FinishOff(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kFinishOff)),
	m_is_stop_all_state	(false),
	m_player			(player)
{

}

player_state::FinishOff::~FinishOff()
{

}

void player_state::FinishOff::Update()
{

}

void player_state::FinishOff::LateUpdate()
{

}

void player_state::FinishOff::Enter()
{

}

void player_state::FinishOff::Exit()
{

}

int player_state::FinishOff::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
