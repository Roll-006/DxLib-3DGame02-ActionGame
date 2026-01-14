#include "stealth_kill_virtual_camera_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"

StealthKillVirtualCameraController::StealthKillVirtualCameraController() :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kStealthKillCutscene),
	m_controller_handle	(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active			(true),
	m_stealth_killer_modeler	(nullptr),
	m_stealth_killable_modeler	(nullptr),
	m_camera			(std::make_shared<VirtualCamera>(ObjName.STEALTH_KILL_VIRTUAL_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform		(std::make_shared<Transform>())
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<StealthKillEvent>		(this, &StealthKillVirtualCameraController::SetStealthKillerModelHandle);
	EventSystem::GetInstance()->Subscribe<OnStealthKillEvent>	(this, &StealthKillVirtualCameraController::SetStealthKillableModelHandle);

	// パラメータ設定
	SetupCamera();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto control_camera = std::dynamic_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(0.5f);
	cinemachine_brain->AddVirtualCamera(m_camera, true);
}

StealthKillVirtualCameraController::~StealthKillVirtualCameraController()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<StealthKillEvent>	(this, &StealthKillVirtualCameraController::SetStealthKillerModelHandle);
	EventSystem::GetInstance()->Unsubscribe<OnStealthKillEvent>	(this, &StealthKillVirtualCameraController::SetStealthKillableModelHandle);

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(1.0f);
	cinemachine_brain->RemoveVirtualCamera(m_camera->GetCameraHandle());

	// 操作カメラを復帰させる
	const auto control_cameras_controller = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	if (control_cameras_controller)
	{
		control_cameras_controller->Activate();
		control_cameras_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();
	}

	m_stealth_killer_modeler	= nullptr;
	m_stealth_killable_modeler	= nullptr;

	// 演出が終了したことを通知
	const EndCutsceneEvent event{};
	EventSystem::GetInstance()->Publish(event);
}

void StealthKillVirtualCameraController::Init()
{
	
}

void StealthKillVirtualCameraController::Update()
{
	if (!IsActive()) { return; }
}

void StealthKillVirtualCameraController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAimTransform();
}

VirtualCameraControllerKind StealthKillVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCamera> StealthKillVirtualCameraController::GetHaveVirtualCamera(const std::string& name) const
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

std::vector<std::shared_ptr<VirtualCamera>> StealthKillVirtualCameraController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{ m_camera };
}


#pragma region Event
void StealthKillVirtualCameraController::SetStealthKillerModelHandle(const StealthKillEvent& event)
{
	m_stealth_killer_modeler = event.modeler;
}

void StealthKillVirtualCameraController::SetStealthKillableModelHandle(const OnStealthKillEvent& event)
{
	m_stealth_killable_modeler = event.modeler;
}
#pragma endregion


#pragma region カメラ設定
void StealthKillVirtualCameraController::SetupCamera()
{
	m_camera->SetPriority(10);
	m_camera->AttachTarget(m_aim_transform);
	m_camera->GetBody()->SetFollowOffset(kFollowOffset);
	m_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffset);
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void StealthKillVirtualCameraController::CalcAimTransform()
{
	if (!m_stealth_killer_modeler || !m_stealth_killable_modeler) { return; }

	auto	   killer_m		= MV1GetFrameLocalWorldMatrix(m_stealth_killer_modeler	->GetModelHandle(), MV1SearchFrame(m_stealth_killer_modeler	 ->GetModelHandle(), FramePath.HEAD_TOP_END));
	auto	   killable_m	= MV1GetFrameLocalWorldMatrix(m_stealth_killable_modeler->GetModelHandle(), MV1SearchFrame(m_stealth_killable_modeler->GetModelHandle(), FramePath.HEAD_TOP_END));
	const auto killer_pos	= matrix::GetPos(killer_m);
	const auto killable_pos	= matrix::GetPos(killable_m);

	// 基準となるトランスフォームを設定
	const auto center_pos	= (killer_pos + killable_pos) * 0.5f;
	m_aim_transform->SetPos(CoordinateKind::kWorld, center_pos);
	m_aim_transform->SetRot(CoordinateKind::kWorld, matrix::GetRotMatrix(killer_m * MGetRotY(90.0f * math::kDegToRad)));
}
#pragma endregion
