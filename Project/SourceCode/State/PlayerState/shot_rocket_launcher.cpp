#include "shot_rocket_launcher.hpp"

player_state::ShotRocketLauncher::ShotRocketLauncher() :
	WeaponActionStateBase				(static_cast<int>(player_state::WeaponActionStateKind::kShotRocketLauncher)),
	m_subject							(std::make_shared<Subject<ShotRocketLauncher>>()),
	m_rocket_launcher_camera_controller	(nullptr),
	m_wait_timer						(0.0f),
	m_was_shot							(false)
{
	EffectManager  ::GetInstance()->AddToSubject<ShotRocketLauncher>(m_subject);
	GameTimeManager::GetInstance()->GetTimeScaleController()->AddToSubject<ShotRocketLauncher>(m_subject);
}

player_state::ShotRocketLauncher::~ShotRocketLauncher()
{

}

void player_state::ShotRocketLauncher::Update(Player* obj)
{
	m_wait_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleController::LayerKind::kNoneScale);
	
	obj->SetLookDirOffsetValueForAim();
	//obj->DirOfCameraForward();

	const auto gun = std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	gun->CalcShotTimer();
}

void player_state::ShotRocketLauncher::LateUpdate(Player* obj)
{
	const auto roket_launcher = std::static_pointer_cast<RocketLauncher>(obj->GetCurrentHeldWeapon());
	const auto camera		  = ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);

	// ボーン位置補正
	obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), roket_launcher->GetAimDir());

	obj->GetCurrentHeldWeapon()->TrackOwnerHand();

	roket_launcher->CalcDiffusionRange();
	roket_launcher->CalcTargetPos();
	roket_launcher->SetAimDir  (roket_launcher->GetAimDir());
	roket_launcher->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));

	// ショット
	if (m_wait_timer > kShotWaitTime && !m_was_shot)
	{
		roket_launcher->OnShot();
		
		// 各オブザーバーへ通知
		const RocketLauncherShotData		data  = {roket_launcher->GetOwnerName(), roket_launcher->GetExhaustVentTransform()};
		const Event<RocketLauncherShotData> event = { EventKind::kRocketLauncherShot, data };
		m_subject->Notify(event);

		m_was_shot = true;
	}
}

void player_state::ShotRocketLauncher::Enter(Player* obj)
{
	m_wait_timer = 0.0f;
	m_was_shot	 = false;

	// 演出用カメラを生成
	const auto camera_manager = CameraManager::GetInstance();
	m_rocket_launcher_camera_controller = std::make_shared<RocketLauncherVirtualCameraController>(*obj);
	camera_manager->AddVirtualCameraController(m_rocket_launcher_camera_controller);

	obj->DetachWeapon(obj->GetCurrentEquipWeapon());
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::ShotRocketLauncher::Exit(Player* obj)
{
	// 演出用カメラを削除
	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->RemoveVirtualCameraController(m_rocket_launcher_camera_controller);
	m_rocket_launcher_camera_controller = nullptr;

	// 操作カメラを復帰
	const auto camera_control	  = CameraManager::GetInstance()->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	const auto control_all_camra  = camera_control->GetHaveAllVirtualCamera();
	camera_control->Activate();
	for (const auto& camera : control_all_camra)
	{
		camera->Activate();
	}

	// 演出終了後にリコイル処理
	//const auto gun = std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	//std::static_pointer_cast<ControlVirtualCamerasController>(camera_control)->OnRecoil(*gun.get());

	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon());
}

std::shared_ptr<IState<Player>> player_state::ShotRocketLauncher::ChangeState(Player* obj)
{
	const auto state_controller = obj->GetStateController();

	// 銃エイミング状態
	if (m_rocket_launcher_camera_controller->IsEndExitRot())
	{
		return state_controller->GetState<AimGun, Player>();
	}

	return nullptr;
}
