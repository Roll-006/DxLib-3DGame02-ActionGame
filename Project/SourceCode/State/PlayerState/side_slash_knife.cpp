#include "side_slash_knife.hpp"

player_state::SideSlashKnife::SideSlashKnife()
{

}

player_state::SideSlashKnife::~SideSlashKnife()
{

}

void player_state::SideSlashKnife::Update(const Player* obj)
{

}

void player_state::SideSlashKnife::Enter(const Player* obj)
{

}

IState<Player>* player_state::SideSlashKnife::ChangeState(const Player* obj)
{
	auto test = SideSlashKnife();
	return &test;
}
