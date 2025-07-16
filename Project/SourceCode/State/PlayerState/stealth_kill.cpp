#include "stealth_kill.hpp"

player_state::StealthKill::StealthKill() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kStealthKill))
{

}

player_state::StealthKill::~StealthKill()
{

}

void player_state::StealthKill::Update(Player* obj)
{

}

void player_state::StealthKill::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::StealthKill::ChangeState(const Player* obj)
{
	return nullptr;
}
