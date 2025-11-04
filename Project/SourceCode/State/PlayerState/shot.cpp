#include "shot.hpp"

player_state::Shot::Shot() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kShot)),
	m_is_stop_all_state		(false)
{

}

player_state::Shot::~Shot()
{

}

void player_state::Shot::Update(std::shared_ptr<Player>& obj)
{
	obj->StopSearchStealthKillTarget();
	obj->StopSearchMeleeTarget();
	obj->SetLookDirOffsetValueForAim();

	const auto gun = std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	gun->CalcShotTimer();
}

void player_state::Shot::LateUpdate(std::shared_ptr<Player>& obj)
{
	const auto gun		= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	const auto camera	= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	const auto aim_dir	= camera->GetTransform()->GetForward(CoordinateKind::kWorld);

	// ボーン位置補正
	obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), aim_dir);

	obj->GetCurrentHeldWeapon()->TrackOwnerHand();

	gun->CalcCrossHairPos();
	gun->CalcCrossHairRangeShot();
	gun->CalcTargetPos();
	gun->SetAimDir  (aim_dir);
	gun->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));

	if (obj->GetDeltaTime() > 0.0f) { gun->OnShot(); }
	
	// リコイル処理
	const auto camera_controller = CinemachineBrain::GetInstance()->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	std::static_pointer_cast<ControlVirtualCamerasController>(camera_controller)->OnRecoil(*gun.get());
}

void player_state::Shot::Enter(std::shared_ptr<Player>& obj)
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	const auto rot_camera = camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA);
	const auto aim_camera = camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA);

	if (rot_camera) { rot_camera->Deactivate(); }
	if (aim_camera) { aim_camera->Activate(); }

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	obj->HoldWeapon	 (obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

void player_state::Shot::Exit(std::shared_ptr<Player>& obj)
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	const auto rot_camera = camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA);
	const auto aim_camera = camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA);

	if (rot_camera) { rot_camera->Activate(); }
	if (aim_camera) { aim_camera->Deactivate(); }

	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

std::shared_ptr<IState<Player>> player_state::Shot::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// 銃エイミング状態
	return state_controller->GetState<AimGun, Player>();
}
