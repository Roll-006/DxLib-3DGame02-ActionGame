#include "escape.hpp"

player_state::Escape::Escape(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kEscape)),
	m_is_stop_all_state	(true),
	m_player			(player)
{

}

player_state::Escape::~Escape()
{

}

void player_state::Escape::Update()
{
	obj->ActivateInvincibleForcibly();
}

void player_state::Escape::LateUpdate()
{

}

void player_state::Escape::Enter()
{
	
}

void player_state::Escape::Exit()
{

}

int player_state::Escape::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
