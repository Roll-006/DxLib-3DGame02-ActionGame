#include "escape_with_knife.hpp"

player_state::EscapeWithKnife::EscapeWithKnife() :
	SpecialStateBase(static_cast<int>(player_state::SpecialStateKind::kEscapeWithKnife))
{

}

player_state::EscapeWithKnife::~EscapeWithKnife()
{

}

void player_state::EscapeWithKnife::Update(Player* obj)
{

}

void player_state::EscapeWithKnife::Enter(Player* obj)
{

}

void player_state::EscapeWithKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::EscapeWithKnife::ChangeState(const Player* obj)
{
	return nullptr;
}
