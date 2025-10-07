#include "escape_with_knife.hpp"

player_state::EscapeWithKnife::EscapeWithKnife() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kEscapeWithKnife)),
	m_is_stop_all_state	(false)
{

}

player_state::EscapeWithKnife::~EscapeWithKnife()
{

}

void player_state::EscapeWithKnife::Update(std::shared_ptr<Player>& obj)
{

}

void player_state::EscapeWithKnife::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::EscapeWithKnife::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::EscapeWithKnife::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::EscapeWithKnife::ChangeState(std::shared_ptr<Player>& obj)
{
	return nullptr;
}
