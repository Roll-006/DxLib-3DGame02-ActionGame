#include "crouch_turn_around.hpp"

player_state::CrouchTurnAround::CrouchTurnAround() :
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kCrouchTurnAround))
{

}

player_state::CrouchTurnAround::~CrouchTurnAround()
{

}

void player_state::CrouchTurnAround::Update(Player* obj)
{

}

void player_state::CrouchTurnAround::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::CrouchTurnAround::ChangeState(const Player* obj)
{
	return nullptr;
}
