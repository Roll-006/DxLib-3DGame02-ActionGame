#include "control_virtual_camera_base.hpp"

ControlVirtualCameraBase::ControlVirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind) :
	VirtualCameraBase		(name, camera_kind),
	m_move_dir				(v3d::GetZeroV()),
	m_velocity				(v3d::GetZeroV()),
	m_init_angle_speed		(0.0f),
	m_init_end_threshold	(0.0f),
	m_is_input				(false, false, false, false),
	m_is_init_aiming		(false)
{
	m_input_angle[TimeKind::kCurrent] = m_input_angle[TimeKind::kNext] = v3d::GetZeroV();
}
