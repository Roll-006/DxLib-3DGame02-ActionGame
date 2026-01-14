#include "rocket_launcher_virtual_cameras_controller.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../UI/ui_drawer.hpp"
#include "../Object/player.hpp"

RocketLauncherVirtualCamerasController::RocketLauncherVirtualCamerasController(Player& player) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kRocketLauncherCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_player						(player),
	m_rocket_bomb					(nullptr),
	m_enter_rot_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ENTER_ROT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_zoom_in_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ZOOM_IN_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_zoom_out_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ZOOM_OUT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_exit_rot_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_EXIT_ROT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_rot_camera_aim_transform		(std::make_shared<Transform>()),
	m_zoom_camera_aim_transform		(std::make_shared<Transform>()),
	m_follow_offset_for_zoom_in		(kFirstFollowOffsetForZoomInCamera),
	m_follow_offset_for_zoom_out	(kFirstFollowOffsetForZoomOutCamera),
	m_follow_offset_for_exit_rot	(v3d::GetZeroV()),
	m_rot_camera_angle				(kFirstAngleForEnterRotCamera),
	m_zoom_in_wait_timer			(0.0f),
	m_zoom_out_timer				(0.0f),
	m_test_timer					(0.0f)
{
	// パラメータ設定
	SetupForEnterRotCamera();
	SetupForZoomInCamera();
	SetupForZoomOutCamera();
	SetupForExitRotCamera();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();

	const auto control_camera = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(1.0f);
	cinemachine_brain->AddVirtualCamera(m_enter_rot_camera,	true);
	cinemachine_brain->AddVirtualCamera(m_zoom_in_camera,	false);
	cinemachine_brain->AddVirtualCamera(m_zoom_out_camera,	false);
	cinemachine_brain->AddVirtualCamera(m_exit_rot_camera,	false);

	// 演出開始通知
	const StartRocketLauncherCutsceneEvent event{};
	EventSystem::GetInstance()->Publish(event);

	const auto time_manager = GameTimeManager::GetInstance();
	time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,	0.0f);
	time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer,	0.005f);
	time_manager->SetTimeScale(TimeScaleLayerKind::kEffect,	0.0f);

	// MEMO : この段階では操作カメラのトランスフォームの値は生存
}

RocketLauncherVirtualCamerasController::~RocketLauncherVirtualCamerasController()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_enter_rot_camera	->GetCameraHandle());
	cinemachine_brain->RemoveVirtualCamera(m_zoom_in_camera		->GetCameraHandle());
	cinemachine_brain->RemoveVirtualCamera(m_zoom_out_camera	->GetCameraHandle());
	cinemachine_brain->RemoveVirtualCamera(m_exit_rot_camera	->GetCameraHandle());
}

void RocketLauncherVirtualCamerasController::Init()
{
	
}

void RocketLauncherVirtualCamerasController::Update()
{
	if (!IsActive()) { return; }
}

void RocketLauncherVirtualCamerasController::LateUpdate()
{
	if (!IsActive()) { return; }

	m_player.GetModeler()->ApplyMatrix();

	CalcAimTransformForEnterRotCamera();
	CalcAimTransformForZoomInCamera();
	CalcAimTransformForZoomOutCamera();
	CalcAimTransformForExitRotCamera();
}

VirtualCameraControllerKind RocketLauncherVirtualCamerasController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCamera> RocketLauncherVirtualCamerasController::GetHaveVirtualCamera(const std::string& name) const
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera = cinemachine_brain->GetVirtualCamera(name);
	if (!camera) { return nullptr; }

	if (   camera == m_enter_rot_camera
		|| camera == m_zoom_in_camera
		|| camera == m_zoom_out_camera
		|| camera == m_exit_rot_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCamera>> RocketLauncherVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{m_enter_rot_camera, m_zoom_in_camera, m_zoom_out_camera};
}


#pragma region カメラ設定
void RocketLauncherVirtualCamerasController::SetupForEnterRotCamera()
{
	m_enter_rot_camera->SetPriority(10);
	m_enter_rot_camera->AttachTarget(m_rot_camera_aim_transform);
	m_enter_rot_camera->GetBody()->SetFollowOffset(kFirstFollowOffsetForEnterRotCamera);
	m_enter_rot_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForEnterRotCamera);
}

void RocketLauncherVirtualCamerasController::SetupForZoomInCamera()
{
	m_zoom_in_camera->SetPriority(11);
	m_zoom_in_camera->AttachTarget(m_zoom_camera_aim_transform);
	m_zoom_in_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_in);
	m_zoom_in_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForZoomInCamera);
}

void RocketLauncherVirtualCamerasController::SetupForZoomOutCamera()
{
	m_zoom_out_camera->SetPriority(12);
	m_zoom_out_camera->AttachTarget(m_zoom_camera_aim_transform);
	m_zoom_out_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_out);
	m_zoom_out_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForZoomOutCamera);
}

void RocketLauncherVirtualCamerasController::SetupForExitRotCamera()
{
	m_exit_rot_camera->SetPriority(13);
	m_exit_rot_camera->AttachTarget(m_rot_camera_aim_transform);
	m_exit_rot_camera->GetBody()->SetFollowOffset(m_follow_offset_for_exit_rot);
	m_exit_rot_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForExitRotCamera);
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void RocketLauncherVirtualCamerasController::CalcAimTransformForEnterRotCamera()
{
	if (!m_enter_rot_camera->IsActive()) { return; }

	// ズームインへ移行
	if (m_rot_camera_angle.y == -DX_PI_F)
	{
		m_enter_rot_camera->Deactivate();
		m_zoom_in_camera->Activate();

		const auto cinemachine_brain = CinemachineBrain::GetInstance();
		cinemachine_brain->SetBlendTime(0.2f);
	}

	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	hand_index		= MV1SearchFrame(model_handle, FramePath.RIGHT_HAND);
	auto		hand_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, hand_index);
	const auto	aim_pos			= matrix::GetPos(hand_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_angle) * offset_rot * matrix::GetRotMatrix(hand_world_m);

	const auto  axis = math::ConvertRotMatrixToAxis(aim_rot);
	axis::Draw(axis, aim_pos, 100);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_rot_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_rot_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// 回転量を計算
	const float acc = kEnterRotAcceleration * m_enter_rot_camera->GetDeltaTime();
	math::Decrease(m_rot_camera_angle.y, acc, -DX_PI_F);
}

void RocketLauncherVirtualCamerasController::CalcAimTransformForZoomInCamera()
{
	if (!m_zoom_in_camera->IsActive()) { return; }

	const float delta_time = m_zoom_in_camera->GetDeltaTime();
	m_zoom_in_wait_timer += delta_time;

	// ズームアウトへ移行
	if (std::abs(m_follow_offset_for_zoom_in.z - kDestinationFollowOffsetForZoomInCamera.z) < 1.0f)
	{
		m_zoom_in_camera->Deactivate();
		m_zoom_out_camera->Activate();

		m_follow_offset_for_zoom_out = VGet(0.0f, 0.0f, m_follow_offset_for_zoom_in.z);

		const auto cinemachine_brain = CinemachineBrain::GetInstance();
		cinemachine_brain->SetBlendTime(2.0f);
	}

	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	hand_index		= MV1SearchFrame(model_handle, FramePath.RIGHT_HAND);
	auto		hand_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, hand_index);
	const auto	aim_pos			= matrix::GetPos(hand_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= MGetRotY(DX_PI_F) * offset_rot * matrix::GetRotMatrix(hand_world_m);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_zoom_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_zoom_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// オフセット値を更新
	if (m_zoom_in_wait_timer > 0.2f)
	{
		m_follow_offset_for_zoom_in.z = math::GetDampedValue(m_follow_offset_for_zoom_in.z, kDestinationFollowOffsetForZoomInCamera.z, kZoomInDamping, delta_time);
		m_zoom_in_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_in);
	}
}

void RocketLauncherVirtualCamerasController::CalcAimTransformForZoomOutCamera()
{
	if (!m_zoom_out_camera->IsActive()) { return; }

	// 演出中に弾丸が消失した場合は即座に演出を中止
	if (!m_rocket_bomb->IsActive())
	{
		Exit();
		return;
	}

	m_zoom_out_timer += m_zoom_out_camera->GetDeltaTime();
	m_test_timer	 += m_zoom_out_camera->GetDeltaTime();

	// 復帰回転カメラへ移行
	if (m_zoom_out_timer >= kZoomOutTime)
	{
		m_zoom_out_camera->Deactivate();
		m_exit_rot_camera->Activate();

		m_follow_offset_for_exit_rot = m_follow_offset_for_zoom_out;

		const auto cinemachine_brain = CinemachineBrain::GetInstance();
		cinemachine_brain->SetBlendTime(0.6f);
	}

	// カメラの追跡対象となるトランスフォームの情報を更新
	const auto tarnsform = m_rocket_bomb->GetTransform();
	auto aim_rot = math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_angle) * tarnsform->GetRotMatrix(CoordinateKind::kWorld);
	m_zoom_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_zoom_camera_aim_transform->SetPos(CoordinateKind::kWorld, tarnsform->GetPos(CoordinateKind::kWorld));

	if (m_test_timer > 1.0f)
	{
		// オフセット値を更新
		// 相対的にはズームアウトとして移動するが、弾丸を基準とした場合ズームインとして移動させる
		m_follow_offset_for_zoom_out.z += kZoomOutSpeed;
	}

	m_zoom_out_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_out);
}

void RocketLauncherVirtualCamerasController::CalcAimTransformForExitRotCamera()
{
	if (!m_exit_rot_camera->IsActive()) { return; }

	// 演出中に弾丸が消失した場合は即座に演出を中止
	if (!m_rocket_bomb->IsActive())
	{
		Exit();
		return;
	}

	// カメラの追跡対象となるトランスフォームの情報を更新
	const auto tarnsform = m_rocket_bomb->GetTransform();
	auto aim_rot = math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_angle) * tarnsform->GetRotMatrix(CoordinateKind::kWorld);
	m_rot_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_rot_camera_aim_transform->SetPos(CoordinateKind::kWorld, tarnsform->GetPos(CoordinateKind::kWorld));

	// 回転量を計算
	const float acc = kExitRotAcceleration * m_exit_rot_camera->GetDeltaTime();
	math::Decrease(m_rot_camera_angle.y, acc, -DX_TWO_PI_F);

	// オフセット値を計算
	m_follow_offset_for_exit_rot.z -= kExitRotLeaveSpeed * m_exit_rot_camera->GetDeltaTime();
	m_exit_rot_camera->GetBody()->SetFollowOffset(m_follow_offset_for_exit_rot);

	if (IsEndExitRot())
	{
		Exit();
	}
}
#pragma endregion


void RocketLauncherVirtualCamerasController::Exit()
{
	// 通知
	const EndRocketLauncherCutsceneEvent event{};
	EventSystem::GetInstance()->Publish(event);

	const auto time_manager = GameTimeManager::GetInstance();
	time_manager->InitTimeScale();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(0.75f);

	m_enter_rot_camera->Deactivate();
	m_zoom_in_camera  ->Deactivate();
	m_zoom_out_camera ->Deactivate();
	m_exit_rot_camera ->Deactivate();

	// 操作カメラの復帰
	const auto control_cameras_controller = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	control_cameras_controller->Activate();
	control_cameras_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();

	m_rot_camera_angle.y = -DX_TWO_PI_F;
}
