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
	obj->NotifyShotRocketLauncher();

	const auto gun		= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	const auto camera	= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	const auto aim_dir	= camera->GetTransform()->GetForward(CoordinateKind::kWorld);

	// ƒ{[ƒ“ˆÊ’u•â³
	obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), aim_dir);

	obj->GetCurrentHeldWeapon()->LateUpdate();

	gun->CalcDiffusionRange();
	gun->CalcTargetPos();
	gun->SetAimDir  (aim_dir);
	gun->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));
	gun->OnShot();
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
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	return state_controller->GetState<AimGun, Player>();

	//return nullptr;
}
