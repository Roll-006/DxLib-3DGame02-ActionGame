#include "special_null.hpp"

player_state::SpecialNull::SpecialNull() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kSpecialNull))
{

}

player_state::SpecialNull::~SpecialNull()
{

}

void player_state::SpecialNull::Update(std::shared_ptr<Player> obj)
{

}

void player_state::SpecialNull::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::SpecialNull::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::SpecialNull::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::SpecialNull::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
