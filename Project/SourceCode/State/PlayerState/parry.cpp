#include "parry.hpp"

player_state::Parry::Parry() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kParry)),
	m_is_all_stop			(false)
{

}

player_state::Parry::~Parry()
{

}

void player_state::Parry::Update(std::shared_ptr<Player>& obj)
{

}

void player_state::Parry::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Parry::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::Parry::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::Parry::ChangeState(std::shared_ptr<Player>& obj)
{
	return nullptr;
}
