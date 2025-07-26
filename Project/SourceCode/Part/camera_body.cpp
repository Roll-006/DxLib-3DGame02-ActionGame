#include "camera_body.hpp"

CameraBody::CameraBody(const std::shared_ptr<Transform> owner_transform) :
	m_owner_transform	(owner_transform),
	m_target_transform	(nullptr),
	m_destination_pos	(v3d::GetZeroV()),
	m_current_pos		(v3d::GetZeroV()),
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
	m_destination_pos = m_target_transform->GetPos(CoordinateKind::kWorld);

	// ターゲットの軸をもとに位置を決定
	const auto target_axes = m_target_transform->GetAxes(CoordinateKind::kWorld);
	m_destination_pos += target_axes.x_axis * m_camera_correct_pos.x;
	m_destination_pos += target_axes.y_axis * m_camera_correct_pos.y;
	m_destination_pos += target_axes.z_axis * m_camera_correct_pos.z;

	m_owner_transform->SetPos(CoordinateKind::kWorld, m_destination_pos);
}


void CameraBody::CalcDampedPos()
{
	const auto owner_axes	= m_owner_transform->GetAxes(CoordinateKind::kWorld);
	const auto distance		= m_destination_pos - m_current_pos;

	// カメラの軸に分解（内積）
	float forward_dot		= VDot(distance, owner_axes.z_axis);
	float right_dot			= VDot(distance, owner_axes.x_axis);
	float up_dot			= VDot(distance, owner_axes.y_axis);

	// 各軸の移動量に対して減衰を適用
	float damped_forward	= math::GetDampedValue(0.0f, forward_dot, m_damping.z);
	float damped_right		= math::GetDampedValue(0.0f, right_dot,   m_damping.x);
	float damped_up			= math::GetDampedValue(0.0f, up_dot,      m_damping.y);

	// 4. 減衰移動ベクトルを再合成
	const VECTOR damped_move =
		owner_axes.z_axis * damped_forward +
		owner_axes.x_axis * damped_right +
		owner_axes.y_axis * damped_up;

	m_current_pos += damped_move;

	m_owner_transform->SetPos(CoordinateKind::kWorld, m_current_pos);
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

