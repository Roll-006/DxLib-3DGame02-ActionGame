#include "escape.hpp"

player_state::Escape::Escape() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kEscape)),
	m_is_stop_all_state	(false)
{

}

player_state::Escape::~Escape()
{

}

void player_state::Escape::Update(std::shared_ptr<Player>& obj)
{

}

void player_state::Escape::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Escape::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::Escape::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::Escape::ChangeState(std::shared_ptr<Player>& obj)
{
	return nullptr;
}
