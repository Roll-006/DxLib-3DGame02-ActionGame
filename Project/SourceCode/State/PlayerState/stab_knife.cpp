#include "stab_knife.hpp"

player_state::StabKnife::StabKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kStabKnife))
{

}

player_state::StabKnife::~StabKnife()
{

}

void player_state::StabKnife::Update(Player* obj)
{
	obj->SetLookDirOffsetValueForAim();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::StabKnife::LateUpdate(Player* obj)
{
	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::StabKnife::Enter(Player* obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::StabKnife::Exit(Player* obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::StabKnife::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();

	// ナイフエイミング状態
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody) && obj->GetCurrentEquipKnife())
	{
		return state_controller->GetState<AimKnife, Player>();
	}

	return nullptr;
}
