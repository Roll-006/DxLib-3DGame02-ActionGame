#include "game_clear_virtual_cameras_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"

GameClearVirtualCamerasController::GameClearVirtualCamerasController(const int model_handle, const std::shared_ptr<Transform>& boss_transform) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kGameClearCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_dead_boss_camera				(std::make_shared<VirtualCamera>(ObjName.DEAD_BOSS_VIRTUAL_CAMERA,	  BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_victory_pose_camera			(std::make_shared<VirtualCamera>(ObjName.VICTORY_POSE_VIRTUAL_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_follow_offset					(kFollowOffset),
	m_follow_offset_dir				(v3d::GetNormalizedV(kFollowOffset)),
	m_zoom_in_wait_time				(0.0f),
	m_zoom_in_speed					(kZoomInInitialVelocity),
	m_player_modeler				(nullptr),
	m_boss_model_handle				(model_handle),
	m_boss_transform				(boss_transform)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<VictoryPoseEvent>(this, &GameClearVirtualCamerasController::SetVictoryPoseCamera);

	// パラメータ設定
	SetupDeadBossCamera();
	SetupVictoryPoseCamera();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto control_camera = std::dynamic_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(0.0f);
	cinemachine_brain->AddVirtualCamera(m_dead_boss_camera,	   true);
	cinemachine_brain->AddVirtualCamera(m_victory_pose_camera, false);
}

GameClearVirtualCamerasController::~GameClearVirtualCamerasController()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<VictoryPoseEvent>(this, &GameClearVirtualCamerasController::SetVictoryPoseCamera);

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_dead_boss_camera	->GetCameraHandle());
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

	CalcAimTransformForDeadBossCamera();
	CalcAimTranasormForVictoryPoseCamera();
	CalcFollowOffset();
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

	if (camera == m_dead_boss_camera || m_victory_pose_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCamera>> GameClearVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{ m_dead_boss_camera, m_victory_pose_camera };
}

void GameClearVirtualCamerasController::SetVictoryPoseCamera(const VictoryPoseEvent& event)
{
	m_player_modeler	= event.modeler;
	m_player_transform	= event.transform;

	m_dead_boss_camera	 ->Deactivate();
	m_victory_pose_camera->Activate();
}


#pragma region カメラ設定
void GameClearVirtualCamerasController::SetupDeadBossCamera()
{
	m_dead_boss_camera->SetPriority(50);
	m_dead_boss_camera->AttachTarget(m_aim_transform);
	m_dead_boss_camera->GetBody()->SetFollowOffset(m_follow_offset);
	m_dead_boss_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffset);
}

void GameClearVirtualCamerasController::SetupVictoryPoseCamera()
{
	m_victory_pose_camera->SetPriority(51);
	m_victory_pose_camera->AttachTarget(m_aim_transform);
	m_victory_pose_camera->GetBody()->SetFollowOffset	 (VGet(-25.0f, -15.0f, -70.0f));
	m_victory_pose_camera->GetAim() ->SetTrackedObjOffset(VGet(  0.0f,  20.0f,   0.0f));
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void GameClearVirtualCamerasController::CalcAimTransformForDeadBossCamera()
{
	if (!m_dead_boss_camera->IsActive()) { return; }

	auto	   frame_m		= MV1GetFrameLocalWorldMatrix(m_boss_model_handle, MV1SearchFrame(m_boss_model_handle, BonePath.HIPS));
	const auto frame_pos	= MGetTranslateElem(frame_m);
	const auto rot_m		= m_boss_transform->GetRotMatrix(CoordinateKind::kWorld) * MGetRotY(180.0f * math::kDegToRad);
	//const auto boss_pos		= m_boss_transform->GetPos(CoordinateKind::kWorld);
	//const auto pos			= VGet(frame_pos.x, boss_pos.y, frame_pos.z);

	// 基準となるトランスフォームを設定
	m_aim_transform->SetRot(CoordinateKind::kWorld, rot_m);
	m_aim_transform->SetPos(CoordinateKind::kWorld, frame_pos);
}

void GameClearVirtualCamerasController::CalcAimTranasormForVictoryPoseCamera()
{
	if (!m_victory_pose_camera->IsActive()) { return; }

	const auto model_handle = m_player_modeler->GetModelHandle();
	auto	   frame_m		= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.HIPS));
	const auto frame_pos	= MGetTranslateElem(frame_m);
	//const auto pos			= m_player_transform->GetPos		(CoordinateKind::kWorld);
	const auto rot_m		= m_player_transform->GetRotMatrix	(CoordinateKind::kWorld) * MGetRotY(DX_PI_F);
	const auto result_pos	= frame_pos + VGet(0.0f, -5.0f, 0.0f);

	m_aim_transform->SetPos(CoordinateKind::kWorld, result_pos);
	m_aim_transform->SetRot(CoordinateKind::kWorld, rot_m);
}

void GameClearVirtualCamerasController::CalcFollowOffset()
{
	if (!m_dead_boss_camera->IsActive()) { return; }

	const auto delta_time = m_dead_boss_camera->GetDeltaTime();

	// ズームイン
	m_zoom_in_wait_time += delta_time;
	if (m_zoom_in_wait_time > kZoomInWaitTime)
	{
		math::Decrease(m_zoom_in_speed, kZoomInDeceleration * delta_time, 0.0f);
		if (m_zoom_in_speed > 0.0f)
		{
			m_follow_offset -= m_follow_offset_dir * m_zoom_in_speed;
		}
	}

	m_dead_boss_camera->GetBody()->SetFollowOffset(m_follow_offset);
}
#pragma endregion
