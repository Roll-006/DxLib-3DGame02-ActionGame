#include "grab_virtual_camera_controller.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"

GrabVirtualCameraController::GrabVirtualCameraController() :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kGrabCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_subject						(std::make_shared<Subject<GrabVirtualCameraController>>()),
	m_grabber_model_handle			(-1),
	m_grabbed_model_handle			(-1),
	m_camera						(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ENTER_ROT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>())
{
	// パラメータ設定
	SetupCamera();
	
	const auto cinemachine_brain = CinemachineBrain::GetInstance();

	// オブザーバーを追加
	const auto control_camera = std::dynamic_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	m_subject->AddObserver(control_camera);

	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(0.5f);
	cinemachine_brain->AddVirtualCamera(m_camera, true);
}

GrabVirtualCameraController::~GrabVirtualCameraController()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->RemoveVirtualCamera(m_camera->GetCameraHandle());

	// 操作カメラを復帰させる
	const auto control_cameras_controller = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	cinemachine_brain->SetBlendTime(1.0f);
	control_cameras_controller->Activate();
	control_cameras_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();

	// 演出が終了したことを通知
	const Event<EndGrabCutsceneData> event = { EventKind::kEndGrabCutscene, {} };
	m_subject->Notify(event);
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

void GrabVirtualCameraController::OnNotify(const IEvent& event)
{
	// 掴んだ
	if (event.GetType() == std::type_index(typeid(GrabData)))
	{
		const auto& grab_event = static_cast<const Event<GrabData>&>(event);
		m_grabber_model_handle = grab_event.data.model_handle;
	}
	// 掴まれた
	if (event.GetType() == std::type_index(typeid(GrabbedData)))
	{
		const auto& grabbed_event = static_cast<const Event<GrabbedData>&>(event);
		m_grabbed_model_handle = grabbed_event.data.model_handle;
	}
}

VirtualCameraControllerKind GrabVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCamera> GrabVirtualCameraController::GetHaveVirtualCamera(const std::string& name) const
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera = cinemachine_brain->GetVirtualCamera(name);

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
	if (m_grabber_model_handle == -1 || m_grabbed_model_handle == -1) { return; }

	auto	   grabber_m	= MV1GetFrameLocalWorldMatrix(m_grabber_model_handle, MV1SearchFrame(m_grabber_model_handle, BonePath.HEAD_TOP_END));
	auto	   grabbed_m	= MV1GetFrameLocalWorldMatrix(m_grabbed_model_handle, MV1SearchFrame(m_grabber_model_handle, BonePath.HEAD_TOP_END));
	const auto grabber_pos	= MGetTranslateElem(grabber_m);
	const auto grabbed_pos	= MGetTranslateElem(grabbed_m);
	const auto grabbed_axes = math::ConvertRotMatrixToAxes(grabbed_m);

	// 基準となるトランスフォームを設定
	const auto center_pos	= (grabber_pos + grabbed_pos) * 0.5f;
	const auto distance		= VSize(grabber_pos - grabbed_pos);
	m_aim_transform->SetPos(CoordinateKind::kWorld, center_pos);
	m_aim_transform->SetRot(CoordinateKind::kWorld, grabbed_axes.x_axis);
}
#pragma endregion
