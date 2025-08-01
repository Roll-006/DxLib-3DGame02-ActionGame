#include "shot.hpp"

player_state::Shot::Shot() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kShot))
{

}

player_state::Shot::~Shot()
{

}

void player_state::Shot::Update(Player* obj)
{
	obj->SetLookDirOffsetValueForAim();
}

void player_state::Shot::LateUpdate(Player* obj)
{

}

void player_state::Shot::Enter(Player* obj)
{

}

void player_state::Shot::Exit(Player* obj)
{

}

std::shared_ptr<IState<Player>> player_state::Shot::ChangeState(const Player* obj)
{
	return nullptr;
}
