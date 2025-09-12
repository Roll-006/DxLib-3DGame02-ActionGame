#include "grab_virtual_camera_controller.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"

GrabVirtualCameraController::GrabVirtualCameraController() :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kGrabCutscene),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	//m_subject						(std::make_shared<Subject<GrabVirtualCameraController>>()),
	m_grabber_model_handle			(-1),
	m_grabbed_model_handle			(-1),
	m_camera						(std::make_shared<VirtualCamera>(ObjName.ROCKET_LAUNCHER_ENTER_ROT_VIRTUAL_CAMERA,	BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>())
{
	// パラメータ設定
	SetupCamera();
	
	const auto cinemachine_brain = CinemachineBrain::GetInstance();

	const auto control_camera = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	control_camera->Deactivate();
	cinemachine_brain->SetBlendTime(0.5f);
	cinemachine_brain->AddVirtualCamera(m_camera, true);
}

GrabVirtualCameraController::~GrabVirtualCameraController()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(1.0f);
	cinemachine_brain->RemoveVirtualCamera(m_camera->GetObjHandle());

	const auto control_cameras_controller = cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
	control_cameras_controller->Activate();
	control_cameras_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA)->Activate();
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
		m_grabber_model_handle = grabbed_event.data.model_handle;
	}
}

VirtualCameraControllerKind GrabVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCameraBase> GrabVirtualCameraController::GetHaveVirtualCamera(const std::string& name) const
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera = cinemachine_brain->GetVirtualCamera(name);

	if (camera == m_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCameraBase>> GrabVirtualCameraController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCameraBase>>{ m_camera };
}


#pragma region カメラ設定
void GrabVirtualCameraController::SetupCamera()
{
	m_camera->SetPriority(8);
	m_camera->AttachTarget(m_aim_transform);
	m_camera->GetBody()->SetFollowOffset(kFollowOffset);
	m_camera->GetAim()->SetTrackedObjOffset(kTrackedObjOffset);
}
#pragma endregion


#pragma region 起点トランスフォームの計算
void GrabVirtualCameraController::CalcAimTransform()
{

}
#pragma endregion
