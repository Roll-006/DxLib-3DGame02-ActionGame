#include "grabbed.hpp"

player_state::Grabbed::Grabbed() :
	ActionStateBase(static_cast<int>(player_state::ActionStateKind::kGrabbed))
{

}

player_state::Grabbed::~Grabbed()
{

}

void player_state::Grabbed::Update(std::shared_ptr<Player> obj)
{
	obj->UpdateGrabbed();
}

void player_state::Grabbed::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::Grabbed::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::Grabbed::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::Grabbed::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
