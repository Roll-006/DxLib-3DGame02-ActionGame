#include "front_kick.hpp"

player_state::FrontKick::FrontKick() :
	ActionStateBase	(static_cast<int>(player_state::ActionStateKind::kFrontKick)),
	m_is_all_stop	(true)
{

}

player_state::FrontKick::~FrontKick()
{

}

void player_state::FrontKick::Update(std::shared_ptr<Player> obj)
{

}

void player_state::FrontKick::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::FrontKick::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::FrontKick::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::FrontKick::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
