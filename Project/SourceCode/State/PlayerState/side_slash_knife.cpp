#include "side_slash_knife.hpp"

player_state::SideSlashKnife::SideSlashKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kSideSlashKnifeLeftToRight))
{

}

player_state::SideSlashKnife::~SideSlashKnife()
{

}

void player_state::SideSlashKnife::Update(Player* obj)
{

}

void player_state::SideSlashKnife::LateUpdate(Player* obj)
{

}

void player_state::SideSlashKnife::Enter(Player* obj)
{

}

void player_state::SideSlashKnife::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::SideSlashKnife::ChangeState(const Player* obj)
{
	return nullptr;
}
