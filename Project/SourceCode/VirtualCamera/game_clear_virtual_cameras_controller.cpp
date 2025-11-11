#include "game_clear_virtual_cameras_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"

GameClearVirtualCamerasController::GameClearVirtualCamerasController(const int model_handle, const std::shared_ptr<Transform>& boss_transform) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kGameClearCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_camera						(std::make_shared<VirtualCamera>(ObjName.DEAD_BOSS_VIRTUAL_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_follow_offset					(kFollowOffset),
	m_follow_offset_dir				(v3d::GetNormalizedV(kFollowOffset)),
	m_zoom_in_wait_time				(0.0f),
	m_zoom_in_speed					(kZoomInInitialVelocity),
	m_model_handle					(model_handle),
	m_boss_transform				(boss_transform)
{
	// パラメータ設定
	SetupCamera();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto control_camera = std::dynamic_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(0.0f);
	cinemachine_brain->AddVirtualCamera(m_camera, true);
}

GameClearVirtualCamerasController::~GameClearVirtualCamerasController()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_camera->GetCameraHandle());
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

	CalcAimTransform();
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

	if (camera == m_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCamera>> GameClearVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{ m_camera };
}

#pragma region カメラ設定
void GameClearVirtualCamerasController::SetupCamera()
{
	m_camera->SetPriority(50);
	m_camera->AttachTarget(m_aim_transform);
	m_camera->GetBody()->SetFollowOffset(m_follow_offset);
	m_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffset);
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void GameClearVirtualCamerasController::CalcAimTransform()
{
	auto	   frame_m		= MV1GetFrameLocalWorldMatrix(m_model_handle, MV1SearchFrame(m_model_handle, BonePath.HIPS));
	const auto frame_pos	= MGetTranslateElem(frame_m);
	const auto rot_m		= m_boss_transform->GetRotMatrix(CoordinateKind::kWorld) * MGetRotY(180.0f * math::kDegToRad);
	//const auto boss_pos		= m_boss_transform->GetPos(CoordinateKind::kWorld);
	//const auto pos			= VGet(frame_pos.x, boss_pos.y, frame_pos.z);

	// 基準となるトランスフォームを設定
	m_aim_transform->SetRot(CoordinateKind::kWorld, rot_m);
	m_aim_transform->SetPos(CoordinateKind::kWorld, frame_pos);
}

void GameClearVirtualCamerasController::CalcFollowOffset()
{
	const auto delta_time = m_camera->GetDeltaTime();

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

	m_camera->GetBody()->SetFollowOffset(m_follow_offset);
}
#pragma endregion
