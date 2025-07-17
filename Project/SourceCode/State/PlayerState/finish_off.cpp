#include "finish_off.hpp"

player_state::FinishOff::FinishOff() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kFinishOff))
{

}

player_state::FinishOff::~FinishOff()
{

}

void player_state::FinishOff::Update(Player* obj)
{

}

void player_state::FinishOff::Enter(Player* obj)
{

}

void player_state::FinishOff::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::FinishOff::ChangeState(const Player* obj)
{
	return nullptr;
}
