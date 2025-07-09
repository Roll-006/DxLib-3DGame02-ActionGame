#include "control_virtual_camera_base.hpp"

ControlVirtualCameraBase::ControlVirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind) :
	VirtualCameraBase	(name, camera_kind),
	m_is_input			(false, false, false, false),
	m_is_init_aiming	(false)
{
	m_angle[TimeKind::kCurrent] = m_angle[TimeKind::kNext] = v3d::GetZeroV();
}
