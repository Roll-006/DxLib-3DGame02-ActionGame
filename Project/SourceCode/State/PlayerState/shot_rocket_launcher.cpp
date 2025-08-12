#include "shot_rocket_launcher.hpp"

player_state::ShotRocketLauncher::ShotRocketLauncher() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kShotRocketLauncher)),
	m_camera_controller		(nullptr),
	m_wait_timer			(0.0f),
	m_was_shot				(false)
{

}

player_state::ShotRocketLauncher::~ShotRocketLauncher()
{

}

void player_state::ShotRocketLauncher::Update(Player* obj)
{
	m_wait_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleController::LayerKind::kNoneScale);
	test		 += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleController::LayerKind::kNoneScale);
	
	obj->SetLookDirOffsetValueForAim();
	//obj->DirOfCameraForward();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::ShotRocketLauncher::LateUpdate(Player* obj)
{
	const auto gun		= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	const auto camera	= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);

	// ボーン位置補正
	obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), gun->GetAimDir());

	obj->GetCurrentHeldWeapon()->LateUpdate();

	gun->CalcDiffusionRange();
	gun->CalcTargetPos();
	gun->SetAimDir  (gun->GetAimDir());
	gun->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));

	// ショット
	if (m_wait_timer > kShotWaitTime && !m_was_shot)
	{
		gun->OnShot();
		obj->NotifyShotRocketLauncher();

		m_was_shot = true;
	}
}

void player_state::ShotRocketLauncher::Enter(Player* obj)
{
	m_wait_timer = 0.0f;
	m_was_shot	 = false;
	test = 0.0f;

	// 演出用カメラを生成
	const auto camera_manager = CameraManager::GetInstance();
	m_camera_controller = std::make_shared<RocketLauncherVirtualCameraController>(*obj);
	camera_manager->AddVirtualCameraController(m_camera_controller);

	obj->DetachWeapon(obj->GetCurrentEquipWeapon());
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::ShotRocketLauncher::Exit(Player* obj)
{
	// 演出用カメラを削除
	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->RemoveVirtualCameraController(m_camera_controller);
	m_camera_controller = nullptr;

	// 演出終了後にリコイル処理
	const auto gun = std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	const auto camera_control = CameraManager::GetInstance()->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	std::static_pointer_cast<ControlVirtualCamerasController>(camera_control)->OnRecoil(*gun.get());

	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon());
}

std::shared_ptr<IState<Player>> player_state::ShotRocketLauncher::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();

	// 銃エイミング状態
	if (test > 30.0f)
	{
		return state_controller->GetState<AimGun, Player>();
	}

	return nullptr;
}
