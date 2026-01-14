#include "grab_virtual_camera_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"

GrabVirtualCameraController::GrabVirtualCameraController() :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kGrabCutscene),
	m_controller_handle	(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active			(true),
	m_grabber_modeler	(nullptr),
	m_grabbed_modeler	(nullptr),
	m_camera			(std::make_shared<VirtualCamera>(ObjName.GRAB_VIRTUAL_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform		(std::make_shared<Transform>())
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<GrabEvent>	(this, &GrabVirtualCameraController::SetGrabberModelHandle);
	EventSystem::GetInstance()->Subscribe<OnGrabEvent>	(this, &GrabVirtualCameraController::SetGrabbedModelHandle);

	// パラメータ設定
	SetupCamera();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto control_camera = std::dynamic_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(0.5f);
	cinemachine_brain->AddVirtualCamera(m_camera, true);
}

GrabVirtualCameraController::~GrabVirtualCameraController()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<GrabEvent>		(this, &GrabVirtualCameraController::SetGrabberModelHandle);
	EventSystem::GetInstance()->Unsubscribe<OnGrabEvent>	(this, &GrabVirtualCameraController::SetGrabbedModelHandle);

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

	m_grabber_modeler = nullptr;
	m_grabbed_modeler = nullptr;

	// 演出が終了したことを通知
	const EndCutsceneEvent event{};
	EventSystem::GetInstance()->Publish(event);
}

void GrabVirtualCameraController::Init()
{
	
}

void GrabVirtualCameraController::Update()
{
	if (!IsActive()) { return; }
}

void GrabVirtualCameraController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAimTransform();
}

VirtualCameraControllerKind GrabVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCamera> GrabVirtualCameraController::GetHaveVirtualCamera(const std::string& name) const
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

std::vector<std::shared_ptr<VirtualCamera>> GrabVirtualCameraController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{ m_camera };
}


#pragma region Event
void GrabVirtualCameraController::SetGrabberModelHandle(const GrabEvent& event)
{
	m_grabber_modeler = event.modeler;
}

void GrabVirtualCameraController::SetGrabbedModelHandle(const OnGrabEvent& event)
{
	m_grabbed_modeler = event.modeler;
}
#pragma endregion


#pragma region カメラ設定
void GrabVirtualCameraController::SetupCamera()
{
	m_camera->SetPriority(10);
	m_camera->AttachTarget(m_aim_transform);
	m_camera->GetBody()->SetFollowOffset(kFollowOffset);
	m_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffset);
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void GrabVirtualCameraController::CalcAimTransform()
{
	if (!m_grabber_modeler || !m_grabbed_modeler) { return; }

	auto	   grabber_m	= MV1GetFrameLocalWorldMatrix(m_grabber_modeler->GetModelHandle(), MV1SearchFrame(m_grabber_modeler->GetModelHandle(), FramePath.HEAD_TOP_END));
	auto	   grabbed_m	= MV1GetFrameLocalWorldMatrix(m_grabbed_modeler->GetModelHandle(), MV1SearchFrame(m_grabbed_modeler->GetModelHandle(), FramePath.HEAD_TOP_END));
	const auto grabber_pos	= matrix::GetPos(grabber_m);
	const auto grabbed_pos	= matrix::GetPos(grabbed_m);
	const auto grabbed_axis = math::ConvertRotMatrixToAxis(grabbed_m);

	// 基準となるトランスフォームを設定
	const auto center_pos	= (grabber_pos + grabbed_pos) * 0.5f;
	m_aim_transform->SetPos(CoordinateKind::kWorld, center_pos);
	m_aim_transform->SetRot(CoordinateKind::kWorld, grabbed_axis.x_axis);
}
#pragma endregion
