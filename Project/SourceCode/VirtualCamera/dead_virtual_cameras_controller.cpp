#include "dead_virtual_cameras_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"

DeadVirtualCamerasController::DeadVirtualCamerasController() :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kDead),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true)
{

}

DeadVirtualCamerasController::~DeadVirtualCamerasController()
{

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
}

VirtualCameraControllerKind DeadVirtualCamerasController::GetVirtualCameraControllerKind() const
{
	return VirtualCameraControllerKind();
}

std::shared_ptr<VirtualCamera> DeadVirtualCamerasController::GetHaveVirtualCamera(const std::string& name) const
{
	return std::shared_ptr<VirtualCamera>();
}

std::vector<std::shared_ptr<VirtualCamera>> DeadVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCamera>>();
}
