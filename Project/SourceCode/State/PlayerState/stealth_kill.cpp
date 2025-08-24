#include "stealth_kill.hpp"

player_state::StealthKill::StealthKill() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kStealthKill))
{

}

player_state::StealthKill::~StealthKill()
{

}

void player_state::StealthKill::Update(std::shared_ptr<Player> obj)
{

}

void player_state::StealthKill::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::StealthKill::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::StealthKill::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::StealthKill::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
