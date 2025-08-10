#include "rocket_launcher_virtual_camera_controller.hpp"
#include "../Manager/camera_manager.hpp"
#include "../Object/player.hpp"

RocketLauncherVirtualCameraController::RocketLauncherVirtualCameraController(Player& player) :
	m_player						(player),
	m_rot_camera					(std::make_shared<VirtualCamera>()),
	m_zoom_out_camera				(std::make_shared<VirtualCamera>()),
	m_rot_camera_aim_transform		(std::make_shared<Transform>()),
	m_zoom_out_camera_aim_transform	(std::make_shared<Transform>()),
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kRocketLauncherShot),
	m_follow_offset_for_zoom_out	(kFollowOffsetForZoomOut),
	m_rot_camera_angle				(kFirstRotCameraAngle),
	m_is_active						(true)
{
	// パラメータ設定
	m_rot_camera	 ->GetBody()->SetFollowOffset	 (kFollowOffsetForRot);
	m_rot_camera	 ->GetAim ()->SetTrackedObjOffset(kTrackedObjOffsetForRot);
	m_zoom_out_camera->GetBody()->SetFollowOffset	 (m_follow_offset_for_zoom_out);
	m_zoom_out_camera->GetAim ()->SetTrackedObjOffset(kTrackedObjOffsetForZoomOut);
	m_rot_camera	 ->SetPriority(2);
	m_zoom_out_camera->SetPriority(3);

	m_zoom_out_camera->Deactivate();

	// 追尾対象をアタッチ
	m_rot_camera	 ->AttachTarget(m_rot_camera_aim_transform);
	m_zoom_out_camera->AttachTarget(m_zoom_out_camera_aim_transform);

	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->AddVirtualCamera(m_rot_camera);
	camera_manager->AddVirtualCamera(m_zoom_out_camera);
	camera_manager->SetBlendTime(0.0f);
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
	CalcAimTransformForZoomOutCamera();
}

VirtualCameraControllerKind RocketLauncherVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

void RocketLauncherVirtualCameraController::CalcAimTransformForRotCamera()
{
	if (m_rot_camera_angle.y == -DX_PI_F)
	{
		// ズームアウトへ移行
		m_zoom_out_camera->Activate();
		const auto camera_manager = CameraManager::GetInstance();
		camera_manager->SetBlendTime(1.0f);
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

void RocketLauncherVirtualCameraController::CalcAimTransformForZoomOutCamera()
{
	if (!m_zoom_out_camera->IsActive()) { return; }

	// TODO : 後に弾丸そのものを追尾するよう変更
	// 追跡するボーンから行列を取得
	const auto	model_handle	= m_player.GetModeler()->GetModelHandle();
	const auto	head_index		= MV1SearchFrame(model_handle, BonePath.RIGHT_HAND);
	auto		head_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, head_index);
	const auto	aim_pos			= MGetTranslateElem(head_world_m);
	const auto  offset_rot		= math::ConvertEulerAnglesToXYZRotMatrix(VGet(-90.0f * math::kDegToRad, -90.0f * math::kDegToRad, 0.0f));
	auto		aim_rot			= MGetRotY(DX_PI_F) * offset_rot * MGetRotElem(head_world_m);

	// カメラの追跡対象となるトランスフォームの情報を更新
	m_zoom_out_camera_aim_transform->SetRot(CoordinateKind::kWorld, aim_rot);
	m_zoom_out_camera_aim_transform->SetPos(CoordinateKind::kWorld, aim_pos);

	// オフセット値を更新
	m_follow_offset_for_zoom_out.z -= kZoomOutkAcceleration * m_zoom_out_camera->GetDeltaTime();
	m_zoom_out_camera->GetBody()->SetFollowOffset(m_follow_offset_for_zoom_out);
}
