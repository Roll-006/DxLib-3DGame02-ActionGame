#include "virtual_camera_base.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"

VirtualCameraBase::VirtualCameraBase(const std::string& name, const BlendActivationPolicyKind blend_policy_kind) :
	ObjBase							(name, ObjTag.VIRTUAL_CAMERA),
	m_body							(std::make_shared<CameraBody> (m_transform)),
	m_aim							(std::make_shared<CameraAim>  (m_transform)),
	m_noise							(std::make_shared<CameraNoise>(m_transform)),
	m_target_transform				(nullptr),
	m_blend_activation_policy_kind	(blend_policy_kind),
	m_priority						(0)
{
	// 処理なし
}

void VirtualCameraBase::AddToObjManager()
{

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

void VirtualCameraBase::SetPriority(const int priority)
{
	m_priority = priority;

	CinemachineBrain::GetInstance()->SortPriority(std::static_pointer_cast<VirtualCameraBase>(shared_from_this()));
}

float VirtualCameraBase::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleController::LayerKind::kCamera);
}

void VirtualCameraBase::CalcTransform()
{
	m_body->CalcPos();
	m_aim ->CalcRot();
	m_body->CalcDampedPos();
	m_aim ->CalcDampedRot();
}
