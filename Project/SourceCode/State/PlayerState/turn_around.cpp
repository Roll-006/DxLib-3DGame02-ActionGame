#include "turn_around.hpp"

player_state::TurnAround::TurnAround() :
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kTurnAround))
{

}

player_state::TurnAround::~TurnAround()
{

}

void player_state::TurnAround::Update(Player* obj)
{

}

void player_state::TurnAround::LateUpdate(Player* obj)
{

}

void player_state::TurnAround::Enter(Player* obj)
{

}

void player_state::TurnAround::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::TurnAround::ChangeState(Player* obj)
{
	return nullptr;
}
