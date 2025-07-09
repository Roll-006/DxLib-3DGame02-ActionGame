#include "rot_control_virtual_camera.hpp"

RotControlVirtualCamera::RotControlVirtualCamera() : 
	ControlVirtualCameraBase(ObjName.ROT_CONTROL_CAMERA, VirtualCameraKind::kRotControl)
{
	m_priority = kPriority;
	m_active_scene_kind.emplace_back(SceneKind::kPlay);
}

RotControlVirtualCamera::~RotControlVirtualCamera()
{

}

void RotControlVirtualCamera::MoveUp()
{

}

void RotControlVirtualCamera::MoveDown()
{

}

void RotControlVirtualCamera::MoveLeft()
{

}

void RotControlVirtualCamera::MoveRight()
{

}

void RotControlVirtualCamera::InitAim()
{

}
