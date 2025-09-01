#include "aim_gun.hpp"

player_state::AimGun::AimGun() :
	WeaponActionStateBase(static_cast<int>(player_state::WeaponActionStateKind::kAimGun))
{
	m_stop_states.emplace_back();
}

player_state::AimGun::~AimGun()
{

}

void player_state::AimGun::Update(std::shared_ptr<Player> obj)
{
	obj->SetLookDirOffsetValueForAim();
	obj->DirOfCameraForward();

	const auto gun = std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	gun->CalcShotTimer();
}

void player_state::AimGun::LateUpdate(std::shared_ptr<Player> obj)
{
	const auto gun			= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	const auto camera		= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	const auto aim_dir		= camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	const auto offset_dir	= (gun->GetFirstShotPos() + gun->GetAimDir() * gun->GetRange()) - gun->GetMuzzleTransform()->GetPos(CoordinateKind::kWorld);

	// ボーン位置補正
	obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), aim_dir);

	obj->GetCurrentHeldWeapon()->TrackOwnerHand();

	gun->CalcDiffusionRange();
	gun->CalcTargetPos();
	gun->SetAimDir  (aim_dir);
	gun->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));
}

void player_state::AimGun::Enter(std::shared_ptr<Player> obj)
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	cinemachine_brain->SetBlendTime(0.3f);
	camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Deactivate();
	camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA)->Activate();

	obj->DetachWeapon(obj->GetCurrentEquipWeapon());
	obj->HoldWeapon(obj->GetCurrentEquipWeapon());
}

void player_state::AimGun::Exit(std::shared_ptr<Player> obj)
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	cinemachine_brain->SetBlendTime(0.3f);
	camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();
	camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA)->Deactivate();

	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon());
}

std::shared_ptr<IState<Player>> player_state::AimGun::ChangeState(std::shared_ptr<Player> obj)
{
	const auto state_controller		= obj->GetStateController();
	const auto command				= CommandHandler::GetInstance();
	const auto gun					= std::static_pointer_cast<GunBase>(obj->GetCurrentHeldWeapon());
	const auto cinemachine_brain	= CinemachineBrain::GetInstance();
	const auto camera_controller	= std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));

	//DrawFormatString(0, 100, 0xffffff, "%d", camera_controller->IsReachedRecoilPeak());

	// 銃装備状態
	if (!command->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent) && camera_controller->IsReachedRecoilPeak())
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// ショット
	if (state_controller->TryPullTrigger(obj))
	{
		if (gun->IsShot() && obj->GetAnimator()->GetBlendRate(AnimatorBase::BodyKind::kUpperBody) >= 1.0f)
		{
			// ロケットランチャーショット(必殺技)
			if (gun->GetGunKind() == GunKind::kRocketLauncher)
			{
				return state_controller->GetState<ShotRocketLauncher, Player>();
			}
			// 通常ショット
			else
			{
				return state_controller->GetState<Shot, Player>();
			}
		}
	}
	// リロード
	if (state_controller->TryReload(obj) && !camera_controller->IsRecoiling())
	{
		return state_controller->GetState<Reload, Player>();
	}
	//// リロード
	//if (gun->GetCurrentRemainingBulletNum() == 0 && command->IsExecuting(CommandKind::kPullTrigger))
	//{
	//	return state_controller->GetState<Reload, Player>();
	//}

	return nullptr;
}
