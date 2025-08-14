#include "rocket_launcher_virtual_camera_controller.hpp"
#include "../Manager/camera_manager.hpp"
#include "../Object/player.hpp"

RocketLauncherVirtualCameraController::RocketLauncherVirtualCameraController(Player& player) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kRocketLauncherCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_player						(player),
	m_subject						(std::make_shared<Subject<RocketLauncherVirtualCameraController>>()),
	m_rot_camera					(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ROT_VIRTUAL_CAMERA,	  BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_zoom_in_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ZOOM_IN_VIRTUAL_CAMERA,  BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_zoom_out_camera				(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ZOOM_OUT_VIRTUAL_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_rot_camera_aim_transform		(std::make_shared<Transform>()),
	m_zoom_camera_aim_transform		(std::make_shared<Transform>()),
	m_follow_offset_for_zoom_in		(kFirstFollowOffsetForZoomInCamera),
	m_follow_offset_for_zoom_out	(v3d::GetZeroV()),
	m_rot_camera_angle				(kFirstAngleForRotCamera),
	m_zoom_in_wait_timer			(0.0f),
	m_zoom_out_speed				(kZoomOutInitialVelocity),
	m_zoom_out_timer				(0.0f)
{
	// パラメータ設定
	SetupForRotCamera();
	SetupForZoomInCamera();
	SetupForZoomOutCamera();

	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->SetBlendTime(1.4f);
	camera_manager->AddVirtualCamera(m_rot_camera,		true);
	camera_manager->AddVirtualCamera(m_zoom_in_camera,	false);
	camera_manager->AddVirtualCamera(m_zoom_out_camera, false);

	GameTimeManager::GetInstance()->GetTimeScaleController()->AddToSubject(m_subject);
}

RocketLauncherVirtualCameraController::~RocketLauncherVirtualCameraController()
{
	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->RemoveVirtualCamera(m_rot_camera->GetObjHandle());
	camera_manager->RemoveVirtualCamera(m_zoom_out_camera ->GetObjHandle());
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

	CalcAimTransformForRotCamera();
	CalcAimTransformForZoomInCamera();
	CalcAimTransformForZoomOutCamera();
}

VirtualCameraControllerKind RocketLauncherVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCameraBase> RocketLauncherVirtualCameraController::GetHaveVirtualCamera(std::string& name) const
{
	const auto camera_manager = CameraManager::GetInstance();
	const auto camera = camera_manager->GetVirtualCamera(name);

	if (   camera == m_rot_camera
		|| camera == m_zoom_in_camera
		|| camera == m_zoom_out_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCameraBase>> RocketLauncherVirtualCameraController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCameraBase>>{m_rot_camera, m_zoom_in_camera, m_zoom_out_camera};
}

void RocketLauncherVirtualCameraController::SetupForRotCamera()
{
	m_rot_camera->SetPriority(8);
	m_rot_camera->AttachTarget(m_rot_camera_aim_transform);
	m_rot_camera->GetBody()->SetFollowOffset(kFirstFollowOffsetForRotCamera);
	m_rot_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffsetForRotCamera);
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

void RocketLauncherVirtualCameraController::CalcAimTransformForRotCamera()
{
	if (!m_rot_camera->IsActive()) { return; }

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
	const auto	head_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		head_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, head_index);
	const auto	aim_pos			= MGetTranslateElem(head_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= math::ConvertEulerAnglesToXYZRotMatrix(m_rot_camera_angle) * offset_rot * MGetRotElem(head_world_m);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_rot_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_rot_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// 回転量を計算
	const float acc = kRotAcceleration * m_rot_camera->GetDeltaTime();
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
	const auto	head_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		head_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, head_index);
	const auto	aim_pos			= MGetTranslateElem(head_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= MGetRotY(DX_PI_F) * offset_rot * MGetRotElem(head_world_m);

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

	// TODO : 後に弾丸そのものを追尾するよう変更(銃の反動アニメーションを付けた際崩壊すると思われるため)
	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	head_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		head_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, head_index);
	const auto	aim_pos			= MGetTranslateElem(head_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= MGetRotY(DX_PI_F) * offset_rot * MGetRotElem(head_world_m);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_zoom_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_zoom_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// オフセット値を更新
	math::Decrease(m_zoom_out_speed, kZoomOutDeceleration * m_zoom_out_camera->GetDeltaTime(), kZoomOutMaxDeceleration);
	m_follow_offset_for_zoom_out.z -= m_zoom_out_speed;
	m_zoom_out_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_out);

	if (IsEndZoomOut())
	{
		// 各オブザーバーへ通知
		const Event<EndRocketLauncherCutsceneData> event = { EventKind::kEndRocketLauncherCutscene, {1.0f} };
		m_subject->Notify(event);
	}
}
