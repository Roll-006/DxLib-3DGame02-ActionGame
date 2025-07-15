#include "special_null.hpp"

player_state::SpecialNull::SpecialNull()
{

}

player_state::SpecialNull::~SpecialNull()
{

}

void player_state::SpecialNull::Update(const Player* obj)
{

}

void player_state::SpecialNull::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::SpecialNull::ChangeState(const Player* obj)
{
	return nullptr;
}
