#include "escape.hpp"

player_state::Escape::Escape() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kEscape))
{

}

player_state::Escape::~Escape()
{

}

void player_state::Escape::Update(Player* obj)
{

}

void player_state::Escape::LateUpdate(Player* obj)
{

}

void player_state::Escape::Enter(Player* obj)
{

}

void player_state::Escape::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Escape::ChangeState(Player* obj)
{
	return nullptr;
}
