#include "reload.hpp"

player_state::Reload::Reload() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kReload))
{

}

player_state::Reload::~Reload()
{

}

void player_state::Reload::Update(Player* obj)
{

}

void player_state::Reload::LateUpdate(Player* obj)
{

}

void player_state::Reload::Enter(Player* obj)
{

}

void player_state::Reload::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Reload::ChangeState(const Player* obj)
{
	return nullptr;
}
