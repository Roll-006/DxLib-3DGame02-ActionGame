#include "suplex.hpp"

player_state::Suplex::Suplex() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kSuplex))
{

}

player_state::Suplex::~Suplex()
{

}

void player_state::Suplex::Update(Player* obj)
{

}

void player_state::Suplex::LateUpdate(Player* obj)
{

}

void player_state::Suplex::Enter(Player* obj)
{

}

void player_state::Suplex::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Suplex::ChangeState(Player* obj)
{
	return nullptr;
}
