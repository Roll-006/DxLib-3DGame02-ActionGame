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

void player_state::Suplex::Enter(const Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Suplex::ChangeState(const Player* obj)
{
	return nullptr;
}
