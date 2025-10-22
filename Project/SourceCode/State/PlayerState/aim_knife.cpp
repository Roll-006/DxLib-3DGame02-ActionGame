#include "aim_knife.hpp"

player_state::AimKnife::AimKnife() :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kAimKnife)),
	m_is_stop_all_state		(false)
{

}

player_state::AimKnife::~AimKnife()
{

}

void player_state::AimKnife::Update(std::shared_ptr<Player>& obj)
{
	obj->AllowCalcLookDir();
	obj->SetLookDirOffsetValueForAim();
	obj->DirOfCameraForward();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::AimKnife::LateUpdate(std::shared_ptr<Player>& obj)
{
	const auto camera	= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	const auto aim_dir	= camera->GetTransform()->GetForward(CoordinateKind::kWorld);

	// ボーン位置補正
	obj->GetBonePosCorrector()->CorrectAimPoseBonePos(obj->GetModeler()->GetModelHandle(), aim_dir);

	obj->GetCurrentHeldWeapon()->TrackOwnerHand();
}

void player_state::AimKnife::Enter(std::shared_ptr<Player>& obj)
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Deactivate();
	camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA)->Activate();
	
	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::AimKnife::Exit(std::shared_ptr<Player>& obj)
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	cinemachine_brain->SetBlendTime(0.3f);
	camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();
	camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA)->Deactivate();
	
	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

std::shared_ptr<IState<Player>> player_state::AimKnife::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	const auto command			= CommandHandler::GetInstance();
	const auto weapon_kind		= obj->GetCurrentHeldWeaponKind();

	// 銃装備状態
	if (weapon_kind == WeaponKind::kGun)
	{
		return state_controller->GetState<EquipGun, Player>();
	}
	// ナイフ装備状態
	if (!command->IsExecute(CommandKind::kAimKnife, TimeKind::kCurrent))
	{
		return state_controller->GetState<EquipKnife, Player>();
	}
	// 突き刺す
	if (command->IsExecute(CommandKind::kAttack, TimeKind::kCurrent)
		|| command->IsExecute(CommandKind::kReload, TimeKind::kCurrent))
	{
		return state_controller->GetState<StabKnife, Player>();
	}

	return nullptr;
}
