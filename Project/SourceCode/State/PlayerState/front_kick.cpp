#include "front_kick.hpp"

player_state::FrontKick::FrontKick() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kFrontKick))
{

}

player_state::FrontKick::~FrontKick()
{

}

void player_state::FrontKick::Update(Player* obj)
{

}

void player_state::FrontKick::LateUpdate(Player* obj)
{

}

void player_state::FrontKick::Enter(Player* obj)
{

}

void player_state::FrontKick::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::FrontKick::ChangeState(const Player* obj)
{
	return nullptr;
}
