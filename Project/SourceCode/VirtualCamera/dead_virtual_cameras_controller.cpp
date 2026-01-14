#include "dead_virtual_cameras_controller.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"

DeadVirtualCamerasController::DeadVirtualCamerasController() :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kDeadPlayer),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_dead_camera					(std::make_shared<VirtualCamera>(ObjName.DEAD_PLAYER_VIRTUAL_CAMERA, BlendActivationPolicyKind::kDeactivateAllCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_model_handle					(-1)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<DeadPlayerEvent>(this, &DeadVirtualCamerasController::ActiveDeadCamera);

	// パラメータ設定
	SetupCamera();

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(3.0f);
	cinemachine_brain->AddVirtualCamera(m_dead_camera, true);
}

DeadVirtualCamerasController::~DeadVirtualCamerasController()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>(this, &DeadVirtualCamerasController::ActiveDeadCamera);

	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	cinemachine_brain->SetBlendTime(1.0f);
	cinemachine_brain->RemoveVirtualCamera(m_dead_camera->GetCameraHandle());
}

void DeadVirtualCamerasController::Init()
{

}

void DeadVirtualCamerasController::Update()
{
	if (!IsActive()) { return; }
}

void DeadVirtualCamerasController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAimTransform();
}

void DeadVirtualCamerasController::ActiveDeadCamera(const DeadPlayerEvent& event)
{
	m_model_handle = event.model_handle;
}

VirtualCameraControllerKind DeadVirtualCamerasController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCamera> DeadVirtualCamerasController::GetHaveVirtualCamera(const std::string& name) const
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera = cinemachine_brain->GetVirtualCamera(name);
	if (!camera) { return nullptr; }

	if (camera == m_dead_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCamera>> DeadVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>{ m_dead_camera };
}

void DeadVirtualCamerasController::SetupCamera()
{
	m_dead_camera->SetPriority(10);
	m_dead_camera->AttachTarget(m_aim_transform);
	m_dead_camera->GetBody()->SetFollowOffset(kFollowOffset);
	m_dead_camera->GetAim() ->SetTrackedObjOffset(kTrackedObjOffset);
}

void DeadVirtualCamerasController::CalcAimTransform()
{
	if (m_model_handle == -1) { return; }

	auto	   spine2_m		= MV1GetFrameLocalWorldMatrix(m_model_handle, MV1SearchFrame(m_model_handle, FramePath.SPINE_2));
	const auto spine2_pos	= matrix::GetPos(spine2_m);

	// 基準となるトランスフォームを設定
	m_aim_transform->SetPos(CoordinateKind::kWorld, spine2_pos);
	m_aim_transform->SetRot(CoordinateKind::kWorld, matrix::GetRotMatrix(spine2_m));
}
