#include "parry.hpp"

player_state::Parry::Parry(Player& player) :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kParry)),
	m_is_stop_all_state		(false),
	m_player				(player)
{

}

player_state::Parry::~Parry()
{

}

void player_state::Parry::Update()
{

}

void player_state::Parry::LateUpdate()
{

}

void player_state::Parry::Enter()
{

}

void player_state::Parry::Exit()
{

}

int player_state::Parry::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	return nullptr;
}
