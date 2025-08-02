#include "special_null.hpp"

player_state::SpecialNull::SpecialNull() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kSpecialNull))
{

}

player_state::SpecialNull::~SpecialNull()
{

}

void player_state::SpecialNull::Update(Player* obj)
{

}

void player_state::SpecialNull::LateUpdate(Player* obj)
{

}

void player_state::SpecialNull::Enter(Player* obj)
{

}

void player_state::SpecialNull::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::SpecialNull::ChangeState(Player* obj)
{
	return nullptr;
}
