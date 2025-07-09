#include "virtual_camera_base.hpp"

VirtualCameraBase::VirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind) :
	ObjBase				(name, ObjTag.VIRTUAL_CAMERA),
	m_body				(nullptr),
	m_aim				(nullptr),
	m_noise				(nullptr),
	m_target_transform	(nullptr),
	m_priority			(0),
	m_camera_kind		(camera_kind),
	m_is_active_camera	(true)
{
	// 処理なし
}

void VirtualCameraBase::AttachTarget(const std::shared_ptr<Transform> target_transform)
{

}

void VirtualCameraBase::DetachTarget()
{

}
