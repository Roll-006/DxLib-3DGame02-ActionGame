#include "camera_aim.hpp"

CameraAim::CameraAim(const std::shared_ptr<Transform> camera_transform) :
	m_camera_transform	(camera_transform),
	m_target_transform	(nullptr),
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

void CameraAim::Update()
{

}

void CameraAim::CalcAim()
{

}


#pragma region Attach / Detach
void CameraAim::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform		= target_transform;
	//m_aim_correct			= v3d::GetZeroV();
	//m_horizontal_damping	= 0.0f;
	//m_vertical_damping	= 0.0f;
	m_is_track				= true;
}

void CameraAim::AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& aim_correct, const float horizontal_damping, const float vertical_damping)
{
	m_target_transform		= target_transform;
	m_aim_correct			= aim_correct;
	m_horizontal_damping	= horizontal_damping;
	m_vertical_damping		= vertical_damping;
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

void CameraAim::SetScreen	(const Vector2D<float>& screen)
{
	m_screen = screen;

	m_screen.x = std::clamp(m_screen.x, 0.0f, 1.0f);
	m_screen.y = std::clamp(m_screen.y, 0.0f, 1.0f);
}

void CameraAim::SetDeadZone	(const Vector2D<float>& dead_zone)
{
	m_dead_zone = dead_zone;

	m_dead_zone.x = std::clamp(m_dead_zone.x, 0.0f, 1.0f);
	m_dead_zone.y = std::clamp(m_dead_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetSoftZone	(const Vector2D<float>& soft_zone)
{
	m_soft_zone = soft_zone;

	m_soft_zone.x = std::clamp(m_soft_zone.x, 0.0f, 1.0f);
	m_soft_zone.y = std::clamp(m_soft_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetBias		(const Vector2D<float>& bias)
{
	m_bias = bias;

	m_bias.x = std::clamp(m_bias.x, -0.5f, 0.5f);
	m_bias.y = std::clamp(m_bias.y, -0.5f, 0.5f);
}
#pragma endregion


#pragma region Getter
VECTOR CameraAim::GetAimPos() const
{
	VECTOR look_pos = m_target_transform->GetPos(CoordinateKind::kWorld);

	// カメラの軸をもとに位置を修正
	const auto axes = m_camera_transform->GetAxes(CoordinateKind::kWorld);
	look_pos += axes.x_axis * m_aim_correct.x;
	look_pos += axes.y_axis * m_aim_correct.y;
	look_pos += axes.z_axis * m_aim_correct.z;

	return look_pos;
}
#pragma endregion
