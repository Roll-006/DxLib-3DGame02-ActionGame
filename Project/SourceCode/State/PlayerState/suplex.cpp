#include "suplex.hpp"

player_state::Suplex::Suplex() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kSuplex))
{

}

player_state::Suplex::~Suplex()
{

}

void player_state::Suplex::Update(std::shared_ptr<Player> obj)
{

}

void player_state::Suplex::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::Suplex::Enter(std::shared_ptr<Player> obj)
{

}

void player_state::Suplex::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::Suplex::ChangeState(std::shared_ptr<Player> obj)
{
	return nullptr;
}
