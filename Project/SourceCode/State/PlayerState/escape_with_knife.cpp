#include "escape_with_knife.hpp"

player_state::EscapeWithKnife::EscapeWithKnife(Player& player) :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kEscapeWithKnife)),
	m_is_stop_all_state	(false),
	m_player			(player)
{

}

player_state::EscapeWithKnife::~EscapeWithKnife()
{

}

void player_state::EscapeWithKnife::Update()
{

}

void player_state::EscapeWithKnife::LateUpdate()
{

}

void player_state::EscapeWithKnife::Enter()
{

}

void player_state::EscapeWithKnife::Exit()
{

}

int player_state::EscapeWithKnife::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
