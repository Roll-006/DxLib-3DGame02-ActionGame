#include "roundhouse_kick.hpp"

player_state::RoundhouseKick::RoundhouseKick() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kRoundhouseKick))
{

}

player_state::RoundhouseKick::~RoundhouseKick()
{

}

void player_state::RoundhouseKick::Update(std::shared_ptr<Player> obj)
{

}

void player_state::RoundhouseKick::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::RoundhouseKick::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::RoundhouseKick::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::RoundhouseKick::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
