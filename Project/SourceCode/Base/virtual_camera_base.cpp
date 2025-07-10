#include "virtual_camera_base.hpp"

VirtualCameraBase::VirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind) :
	ObjBase				(name, ObjTag.VIRTUAL_CAMERA),
	m_body				(std::make_shared<CameraBody> (m_transform)),
	m_aim				(std::make_shared<CameraAim>  (m_transform)),
	m_noise				(std::make_shared<CameraNoise>(m_transform)),
	m_target_transform	(nullptr),
	m_priority			(0),
	m_camera_kind		(camera_kind),
	m_is_active_camera	(true)
{
	// 処理なし
}

void VirtualCameraBase::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform = target_transform;

	m_body->AttachTarget(m_target_transform);
	m_aim ->AttachTarget(m_target_transform);
}

void VirtualCameraBase::DetachTarget()
{
	m_target_transform = nullptr;

	m_body->DetachTarget();
	m_aim ->DetachTarget();
}
