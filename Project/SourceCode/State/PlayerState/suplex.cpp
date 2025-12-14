#include "suplex.hpp"

player_state::Suplex::Suplex(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kSuplex)),
	m_is_stop_all_state	(false),
	m_player			(player)
{

}

player_state::Suplex::~Suplex()
{

}

void player_state::Suplex::Update()
{
	m_player.UpdateMelee();
}

void player_state::Suplex::LateUpdate()
{

}

void player_state::Suplex::Enter()
{

}

void player_state::Suplex::Exit()
{

}

int player_state::Suplex::GetNextStateKind()
{
	if (m_player.GetDeltaTime() <= 0.0f) { return static_cast<int>(player_state::ActionStateKind::kNone); }

	return static_cast<int>(player_state::ActionStateKind::kNone);
}
