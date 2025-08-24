#include "finish_off.hpp"

player_state::FinishOff::FinishOff() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kFinishOff))
{

}

player_state::FinishOff::~FinishOff()
{

}

void player_state::FinishOff::Update(std::shared_ptr<Player> obj)
{

}

void player_state::FinishOff::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::FinishOff::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::FinishOff::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::FinishOff::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
