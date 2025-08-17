#include "rocket_launcher_virtual_camera_controller.hpp"
#include "../Manager/camera_manager.hpp"
#include "../UI/ui_drawer.hpp"
#include "../Object/player.hpp"

RocketLauncherVirtualCameraController::RocketLauncherVirtualCameraController(Player& player) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kRocketLauncherCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_player						(player),
	m_subject						(std::make_shared<Subject<RocketLauncherVirtualCameraController>>()),
	m_enter_rot_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ENTER_ROT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_zoom_in_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ZOOM_IN_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_zoom_out_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ZOOM_OUT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_exit_rot_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_EXIT_ROT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_rot_camera_aim_transform		(std::make_shared<Transform>()),
	m_zoom_camera_aim_transform		(std::make_shared<Transform>()),
	m_rocket_bomb_transform			(nullptr),
	m_follow_offset_for_zoom_in		(kFirstFollowOffsetForZoomInCamera),
	m_follow_offset_for_zoom_out	(v3d::GetZeroV()),
	m_follow_offset_for_exit_rot	(v3d::GetZeroV()),
	m_rot_camera_angle				(kFirstAngleForEnterRotCamera),
	m_zoom_in_wait_timer			(0.0f),
	m_zoom_out_speed				(kZoomOutInitialVelocity),
	m_zoom_out_timer				(0.0f)
{
	// パラメータ設定
	SetupForEnterRotCamera();
	SetupForZoomInCamera();
	SetupForZoomOutCamera();
	SetupForExitRotCamera();

	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->GetVirtualCameraController(VirtualCameraControllerKind::kControl)->Deactivate();
	camera_manager->SetBlendTime(1.4f);
	camera_manager->AddVirtualCamera(m_enter_rot_camera,	true);
	camera_manager->AddVirtualCamera(m_zoom_in_camera,		false);
	camera_manager->AddVirtualCamera(m_zoom_out_camera,		false);
	camera_manager->AddVirtualCamera(m_exit_rot_camera,		false);

	// オブザーバー登録
	const auto screen_filter = UIDrawer::GetInstance()->GetUICreator(UICreatorName.SCREEN_FILTER_CREATOR);
	m_subject->AddObserver(std::dynamic_pointer_cast<IObserver>(screen_filter));
	GameTimeManager::GetInstance()->GetTimeScaleController()->AddToSubject(m_subject);

	// 演出開始通知
	const Event<StartRocketLauncherCutsceneData> event = { EventKind::kEndRocketLauncherCutscene, {0.0f, 0.007f, 0.0f} };
	m_subject->Notify(event);
}

RocketLauncherVirtualCameraController::~RocketLauncherVirtualCameraController()
{
	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->RemoveVirtualCamera(m_enter_rot_camera	->GetObjHandle());
	camera_manager->RemoveVirtualCamera(m_zoom_in_camera	->GetObjHandle());
	camera_manager->RemoveVirtualCamera(m_zoom_out_camera	->GetObjHandle());
	camera_manager->RemoveVirtualCamera(m_exit_rot_camera	->GetObjHandle());
}

void RocketLauncherVirtualCameraController::Init()
{
	
}

void RocketLauncherVirtualCameraController::Update()
{
	if (!IsActive()) { return; }
}

void RocketLauncherVirtualCameraController::LateUpdate()
{
	if (!IsActive()) { return; }

	m_player.GetModeler()->ApplyMatrix();

	CalcAimTransformForEnterRotCamera();
	CalcAimTransformForZoomInCamera();
	CalcAimTransformForZoomOutCamera();
	CalcAimTransformForExitRotCamera();
}

VirtualCameraControllerKind RocketLauncherVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCameraBase> RocketLauncherVirtualCameraController::GetHaveVirtualCamera(std::string& name) const
{
	const auto camera_manager = CameraManager::GetInstance();
	const auto camera = camera_manager->GetVirtualCamera(name);

	if (   camera == m_enter_rot_camera
		|| camera == m_zoom_in_camera
		|| camera == m_zoom_out_camera
		|| camera == m_exit_rot_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCameraBase>> RocketLauncherVirtualCameraController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCameraBase>>{m_enter_rot_camera, m_zoom_in_camera, m_zoom_out_camera};
}


#pragma region カメラ設定
void RocketLauncherVirtualCameraController::SetupForEnterRotCamera()
{
	m_enter_rot_camera->SetPriority(8);
	m_enter_rot_camera->AttachTarget(m_rot_camera_aim_transform);
	m_enter_rot_camera->GetBody()->SetFollowOffset(kFirstFollowOffsetForEnterRotCamera);
	m_enter_rot_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForEnterRotCamera);
}

void RocketLauncherVirtualCameraController::SetupForZoomInCamera()
{
	m_zoom_in_camera->SetPriority(9);
	m_zoom_in_camera->AttachTarget(m_zoom_camera_aim_transform);
	m_zoom_in_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_in);
	m_zoom_in_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForZoomCamera);
}

void RocketLauncherVirtualCameraController::SetupForZoomOutCamera()
{
	m_zoom_out_camera->SetPriority(10);
	m_zoom_out_camera->AttachTarget(m_zoom_camera_aim_transform);
	m_zoom_out_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_out);
	m_zoom_out_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForZoomCamera);
}

void RocketLauncherVirtualCameraController::SetupForExitRotCamera()
{
	m_exit_rot_camera->SetPriority(11);
	m_exit_rot_camera->AttachTarget(m_rot_camera_aim_transform);
	m_exit_rot_camera->GetBody()->SetFollowOffset(kFirstFollowOffsetForExitRotCamera);
	m_exit_rot_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForExitRotCamera);
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void RocketLauncherVirtualCameraController::CalcAimTransformForEnterRotCamera()
{
	if (!m_enter_rot_camera->IsActive()) { return; }

	// ズームインへ移行
	if (m_rot_camera_angle.y == -DX_PI_F)
	{
		m_zoom_in_camera->Activate();

		const auto camera_manager = CameraManager::GetInstance();
		camera_manager->SetBlendTime(0.0f);
	}

	// TODO : 後に弾丸そのものを追尾するよう変更
	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	hand_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		hand_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, hand_index);
	const auto	aim_pos			= MGetTranslateElem(hand_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_angle) * offset_rot * MGetRotElem(hand_world_m);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_rot_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_rot_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// 回転量を計算
	const float acc = kEnterRotAcceleration * m_enter_rot_camera->GetDeltaTime();
	math::Decrease(m_rot_camera_angle.y, acc, -DX_PI_F);
}

void RocketLauncherVirtualCameraController::CalcAimTransformForZoomInCamera()
{
	if (!m_zoom_in_camera->IsActive()) { return; }

	const float delta_time = m_zoom_in_camera->GetDeltaTime();
	m_zoom_in_wait_timer += delta_time;

	// ズームアウトへ移行
	if (std::abs(m_follow_offset_for_zoom_in.z - kDestinationFollowOffsetForZoomInCamera.z) < 1.0f)
	{
		m_zoom_out_camera->Activate();

		m_follow_offset_for_zoom_out = m_follow_offset_for_zoom_in;
	}

	// TODO : 後に弾丸そのものを追尾するよう変更(銃の反動アニメーションを付けた際崩壊すると思われるため)
	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	hand_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		hand_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, hand_index);
	const auto	aim_pos			= MGetTranslateElem(hand_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= MGetRotY(DX_PI_F) * offset_rot * MGetRotElem(hand_world_m);

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

void RocketLauncherVirtualCameraController::CalcAimTransformForZoomOutCamera()
{
	if (!m_zoom_out_camera->IsActive()) { return; }

	m_zoom_out_timer += m_zoom_out_camera->GetDeltaTime();

	// 復帰回転カメラへ移行
	if (m_zoom_out_timer >= kZoomOutTime)
	{
		m_exit_rot_camera->Activate();

		m_follow_offset_for_exit_rot = m_follow_offset_for_zoom_out;

		const auto camera_manager = CameraManager::GetInstance();
		camera_manager->SetBlendTime(2.0f);
	}

	// TODO : 後に弾丸そのものを追尾するよう変更(銃の反動アニメーションを付けた際崩壊すると思われるため)
	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	hand_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		hand_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, hand_index);
	const auto	aim_pos			= MGetTranslateElem(hand_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= MGetRotY(DX_PI_F) * offset_rot * MGetRotElem(hand_world_m);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_zoom_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_zoom_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// オフセット値を更新
	math::Decrease(m_zoom_out_speed, kZoomOutDeceleration * m_zoom_out_camera->GetDeltaTime(), kZoomOutMaxDeceleration);
	m_follow_offset_for_zoom_out.z -= m_zoom_out_speed;
	m_zoom_out_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_out);
}

void RocketLauncherVirtualCameraController::CalcAimTransformForExitRotCamera()
{
	if (!m_exit_rot_camera->IsActive()) { return; }

	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	hand_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		hand_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, hand_index);
	const auto	aim_pos			= MGetTranslateElem(hand_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_angle) * offset_rot * MGetRotElem(hand_world_m);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_rot_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_rot_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// 回転量を計算
	//const float acc = kExitRotAcceleration * m_exit_rot_camera->GetDeltaTime();
	const float acc = 0.5f * m_exit_rot_camera->GetDeltaTime();
	math::Decrease(m_rot_camera_angle.y, acc, -DX_TWO_PI_F);

	// オフセット値を計算
	m_follow_offset_for_exit_rot.z -= 10.0f * m_exit_rot_camera->GetDeltaTime();
	m_exit_rot_camera->GetBody()->SetFollowOffset(m_follow_offset_for_exit_rot);

	if (IsEndExitRot())
	{
		// 各オブザーバーへ通知
		const Event<EndRocketLauncherCutsceneData> event = { EventKind::kEndRocketLauncherCutscene, {1.0f} };
		m_subject->Notify(event);

		const auto camera_manager = CameraManager::GetInstance();
		camera_manager->SetBlendTime(10.0f);
	}
}
#pragma endregion
