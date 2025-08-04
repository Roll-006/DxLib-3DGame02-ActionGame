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

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::Shot::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->LateUpdate();
}

void player_state::Shot::Enter(Player* obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipWeapon());
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::Shot::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon());
}

std::shared_ptr<IState<Player>> player_state::Shot::ChangeState(Player* obj)
{
	return nullptr;
}
