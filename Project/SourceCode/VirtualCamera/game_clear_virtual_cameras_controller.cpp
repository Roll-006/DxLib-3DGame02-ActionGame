#include "game_clear_virtual_cameras_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"

GameClearVirtualCamerasController::GameClearVirtualCamerasController(const int model_handle, const std::shared_ptr<Transform>& transform) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kGameClearCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_victory_pose_camera			(std::make_shared<VirtualCamera>(ObjName.VICTORY_POSE_VIRTUAL_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_player_model_handle			(model_handle),
	m_player_transform				(transform)
{
	// パラメータ設定
	SetupVictoryPoseCamera();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto control_camera = std::dynamic_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(0.0f);
	cinemachine_brain->AddVirtualCamera(m_victory_pose_camera, true);
}

GameClearVirtualCamerasController::~GameClearVirtualCamerasController()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_victory_pose_camera->GetCameraHandle());
	cinemachine_brain->SetBlendTime(1.0f);

	// 操作カメラを復帰させる
	const auto control_cameras_controller = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	if (control_cameras_controller)
	{
		control_cameras_controller->Activate();
		control_cameras_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();
	}

	// 演出が終了したことを通知
	const EndCutsceneEvent event{};
	EventSystem::GetInstance()->Publish(event);
}

void GameClearVirtualCamerasController::Init()
{
	
}

void GameClearVirtualCamerasController::Update()
{
	if (!IsActive()) { return; }
}

void GameClearVirtualCamerasController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAimTranasormForVictoryPoseCamera();
}

VirtualCameraControllerKind GameClearVirtualCamerasController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCamera> GameClearVirtualCamerasController::GetHaveVirtualCamera(const std::string& name) const
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera = cinemachine_brain->GetVirtualCamera(name);
	if (!camera) { return nullptr; }

	if (camera == m_victory_pose_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCamera>> GameClearVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{ m_victory_pose_camera };
}


#pragma region カメラ設定
void GameClearVirtualCamerasController::SetupVictoryPoseCamera()
{
	m_victory_pose_camera->SetPriority(51);
	m_victory_pose_camera->AttachTarget(m_aim_transform);
	m_victory_pose_camera->GetBody()->SetFollowOffset	 (kFollowOffset);
	m_victory_pose_camera->GetAim() ->SetTrackedObjOffset(kTrackedObjOffset);
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void GameClearVirtualCamerasController::CalcAimTranasormForVictoryPoseCamera()
{
	if (!m_victory_pose_camera->IsActive()) { return; }

	auto	   frame_m		= MV1GetFrameLocalWorldMatrix(m_player_model_handle, MV1SearchFrame(m_player_model_handle, BonePath.HIPS));
	const auto frame_pos	= MGetTranslateElem(frame_m);
	const auto rot_m		= m_player_transform->GetRotMatrix	(CoordinateKind::kWorld) * MGetRotY(DX_PI_F);
	const auto result_pos	= frame_pos + VGet(0.0f, -5.0f, 0.0f);

	m_aim_transform->SetPos(CoordinateKind::kWorld, result_pos);
	m_aim_transform->SetRot(CoordinateKind::kWorld, rot_m);
}
#pragma endregion
