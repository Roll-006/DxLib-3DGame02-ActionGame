#include "rot_control_camera.hpp"

RotControlCamera::RotControlCamera() : 
	VirtualCameraBase(ObjName.ROT_CONTROL_CAMERA, VirtualCameraKind::kRotControl)
{
	m_priority = kPriority;
	m_active_scene_kind.emplace_back(SceneKind::kPlay);
}

RotControlCamera::~RotControlCamera()
{

}

void RotControlCamera::MoveUp()
{

}

void RotControlCamera::MoveDown()
{

}

void RotControlCamera::MoveLeft()
{

}

void RotControlCamera::MoveRight()
{

}

void RotControlCamera::InitAngle()
{

}
