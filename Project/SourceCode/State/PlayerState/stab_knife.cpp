#include "stab_knife.hpp"

player_state::StabKnife::StabKnife() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kStabKnife))
{

}

player_state::StabKnife::~StabKnife()
{

}

void player_state::StabKnife::Update(std::shared_ptr<Player> obj)
{
	obj->SetLookDirOffsetValueForAim();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::StabKnife::LateUpdate(std::shared_ptr<Player> obj)
{
	//const auto camera		= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	//const auto aim_dir	= camera->GetTransform()->GetForward(CoordinateKind::kWorld);

	// ボーン位置補正
	//obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), aim_dir);

	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::StabKnife::Enter(std::shared_ptr<Player> obj)
{
	obj->DetachWeapon(obj->GetCurrentEquipKnife());
	obj->HoldWeapon(obj->GetCurrentEquipKnife());
}

void player_state::StabKnife::Exit(std::shared_ptr<Player> obj)
{
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipKnife());
}

std::shared_ptr<IState<Player>> player_state::StabKnife::ChangeState(std::shared_ptr<Player> obj)
{
	const auto state_controller = obj->GetStateController();

	// ナイフエイミング状態
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody) && obj->GetCurrentEquipKnife())
	{
		return state_controller->GetState<AimKnife, Player>();
	}

	return nullptr;
}
