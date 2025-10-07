#include "player_dead.hpp"

player_state::Dead::Dead() :
	ActionStateBase		(static_cast<int>(player_state::ActionStateKind::kDead)),
	m_is_stop_all_state	(false)
{

}

player_state::Dead::~Dead()
{

}

void player_state::Dead::Update(std::shared_ptr<Player>& obj)
{

}

void player_state::Dead::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::Dead::Enter(std::shared_ptr<Player>& obj)
{

}

void player_state::Dead::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::Dead::ChangeState(std::shared_ptr<Player>& obj)
{
	return nullptr;
}
