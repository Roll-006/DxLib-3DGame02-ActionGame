#include "camera_aim.hpp"

CameraAim::CameraAim(const std::shared_ptr<Transform> camera_transform) :
	m_camera_transform	(camera_transform),
	m_target_transform	(nullptr),
	m_target_correct_pos(v3d::GetZeroV()),
	m_aim_correct		(v3d::GetZeroV()),
	m_horizontal_damping(0.0f),
	m_vertical_damping	(0.0f),
	m_screen			(0.5f, 0.5f),
	m_dead_zone			(v2d::GetZeroV<Vector2D<float>>()),
	m_soft_zone			(v2d::GetZeroV<Vector2D<float>>()),
	m_bias				(v2d::GetZeroV<Vector2D<float>>()),
	m_is_track			(false)
{

}

CameraAim::~CameraAim()
{

}


#pragma region Attach / Detach
void CameraAim::AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& target_correct_pos)
{
	m_target_transform		= target_transform;
	m_target_correct_pos	= target_correct_pos;
	m_is_track				= true;
}

void CameraAim::DetachTarget()
{
	m_target_transform	= nullptr;
	m_is_track			= false;
}
#pragma endregion


#pragma region Setter
void CameraAim::SetRot		(const MATRIX& rot_matrix)
{
	m_camera_transform->SetRot(CoordinateKind::kWorld, rot_matrix);
}

void CameraAim::SetHorizontalDamping(const float horizontal_damping)
{
	m_horizontal_damping = std::clamp(horizontal_damping, 0.0f, kMaxDampingNum);
}

void CameraAim::SetVerticalDamping(const float vertical_damping)
{
	m_vertical_damping = std::clamp(vertical_damping, 0.0f, kMaxDampingNum);
}

void CameraAim::SetScreen	(const Vector2D<float>& screen)
{
	m_screen.x		= std::clamp(screen.x, 0.0f, 1.0f);
	m_screen.y		= std::clamp(screen.y, 0.0f, 1.0f);
}

void CameraAim::SetDeadZone	(const Vector2D<float>& dead_zone)
{
	m_dead_zone.x	= std::clamp(dead_zone.x, 0.0f, 1.0f);
	m_dead_zone.y	= std::clamp(dead_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetSoftZone	(const Vector2D<float>& soft_zone)
{
	m_soft_zone.x	= std::clamp(soft_zone.x, 0.0f, 1.0f);
	m_soft_zone.y	= std::clamp(soft_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetBias		(const Vector2D<float>& bias)
{
	m_bias.x		= std::clamp(bias.x, -0.5f, 0.5f);
	m_bias.y		= std::clamp(bias.y, -0.5f, 0.5f);
}
#pragma endregion


#pragma region Getter
VECTOR CameraAim::GetAimPos() const
{
	VECTOR aim_pos = m_target_transform->GetPos(CoordinateKind::kWorld);
	aim_pos += m_target_correct_pos;

	// カメラの軸をもとに位置を修正
	const auto axes = m_camera_transform->GetAxes(CoordinateKind::kWorld);
	aim_pos += axes.x_axis * m_aim_correct.x;
	aim_pos += axes.y_axis * m_aim_correct.y;
	aim_pos += axes.z_axis * m_aim_correct.z;

	return aim_pos;
}
#pragma endregion
