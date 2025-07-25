#include "camera_body.hpp"

CameraBody::CameraBody(const std::shared_ptr<Transform> owner_transform) :
	m_owner_transform	(owner_transform),
	m_target_transform	(nullptr),
	//m_target_correct_pos(v3d::GetZeroV()),
	//m_camera_correct_dir(v3d::GetZeroV()),
	//m_distance_to_target(0.0f),
	m_damping			(v3d::GetZeroV()),
	m_damping_yaw		(0.0f),
	m_is_track			(false)
{
	// 処理なし
}

CameraBody::~CameraBody()
{
	// 処理なし
}

void CameraBody::CalcPos()
{
	m_pos = m_target_transform->GetPos(CoordinateKind::kWorld);

	// ターゲットの軸をもとに位置を決定
	const auto target_axes = m_target_transform->GetAxes(CoordinateKind::kWorld);
	m_pos += target_axes.x_axis * m_camera_correct_pos.x;
	m_pos += target_axes.y_axis * m_camera_correct_pos.y;
	m_pos += target_axes.z_axis * m_camera_correct_pos.z;

	m_owner_transform->SetPos(CoordinateKind::kWorld, m_pos);
}


#pragma region Attach / Detach
void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform		= target_transform;
	m_is_track				= true;
}

void CameraBody::DetachTarget()
{
	m_target_transform		= nullptr;
	m_is_track				= false;
}
#pragma endregion


#pragma region Setter
void CameraBody::SetDamping(const VECTOR& damping)
{
	m_damping.x = std::clamp(damping.x, 0.0f, kMaxDampingNum);
	m_damping.y = std::clamp(damping.y, 0.0f, kMaxDampingNum);
	m_damping.z = std::clamp(damping.y, 0.0f, kMaxDampingNum);
}

void CameraBody::SetDampingYaw(const float damping_yaw)
{
	m_damping_yaw = std::clamp(damping_yaw, 0.0f, kMaxDampingNum);
}
#pragma endregion
