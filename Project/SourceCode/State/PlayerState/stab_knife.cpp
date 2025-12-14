#include "stab_knife.hpp"

player_state::StabKnife::StabKnife(Player& player) :
	WeaponActionStateBase	(static_cast<int>(player_state::WeaponActionStateKind::kStabKnife)),
	m_is_stop_all_state		(false),
	m_player				(player)
{

}

player_state::StabKnife::~StabKnife()
{

}

void player_state::StabKnife::Update()
{
	obj->StopSearchStealthKillTarget();
	obj->StopSearchMeleeTarget();
	obj->SetLookDirOffsetValueForAim();

	obj->GetCurrentHeldWeapon()->Update();
}

void player_state::StabKnife::LateUpdate()
{
	//const auto camera		= ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	//const auto aim_dir	= camera->GetTransform()->GetForward(CoordinateKind::kWorld);

	// ボーン位置補正
	//obj->GetFramePosCorrector()->CorrectAimPoseFramePos(obj->GetModeler()->GetModelHandle(), aim_dir);

	
}

void player_state::StabKnife::Enter()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Deactivate();
	camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA)->Activate();

	obj->DetachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
	obj->HoldWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

void player_state::StabKnife::Exit()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();
	camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA)->Deactivate();

	obj->ReleaseWeapon();
	obj->AttachWeapon(obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub));
}

int player_state::StabKnife::GetNextStateKind()
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();

	// ナイフエイミング状態
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody) && obj->GetCurrentEquipWeapon(WeaponSlotKind::kSub))
	{
		return state_controller->GetState<AimKnife, Player>();
	}

	return nullptr;
}
