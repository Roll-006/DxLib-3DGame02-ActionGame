#include "extension_move_null.hpp"

player_state::ExtensionMoveNull::ExtensionMoveNull()
{

}

player_state::ExtensionMoveNull::~ExtensionMoveNull()
{

}

void player_state::ExtensionMoveNull::Update(const Player* obj)
{

}

void player_state::ExtensionMoveNull::Enter(const Player* obj)
{

}

IState<Player>* player_state::ExtensionMoveNull::ChangeState(const Player* obj)
{
	auto test = ExtensionMoveNull();
	return &test;
}
