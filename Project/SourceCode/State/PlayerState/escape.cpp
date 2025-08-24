#include "escape.hpp"

player_state::Escape::Escape() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kEscape))
{

}

player_state::Escape::~Escape()
{

}

void player_state::Escape::Update(std::shared_ptr<Player> obj)
{

}

void player_state::Escape::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::Escape::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::Escape::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::Escape::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
