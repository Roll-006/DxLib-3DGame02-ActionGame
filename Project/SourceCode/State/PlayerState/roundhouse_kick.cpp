#include "roundhouse_kick.hpp"

player_state::RoundhouseKick::RoundhouseKick() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kRoundhouseKick))
{

}

player_state::RoundhouseKick::~RoundhouseKick()
{

}

void player_state::RoundhouseKick::Update(Player* obj)
{

}

void player_state::RoundhouseKick::Enter(Player* obj)
{

}

void player_state::RoundhouseKick::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::RoundhouseKick::ChangeState(const Player* obj)
{
	return nullptr;
}
