#include "crouch_turn_around.hpp"

player_state::CrouchTurnAround::CrouchTurnAround() :
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kCrouchTurnAround))
{

}

player_state::CrouchTurnAround::~CrouchTurnAround()
{

}

void player_state::CrouchTurnAround::Update(std::shared_ptr<Player> obj)
{

}

void player_state::CrouchTurnAround::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::CrouchTurnAround::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::CrouchTurnAround::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::CrouchTurnAround::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
