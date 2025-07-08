#include "camera_aim.hpp"

CameraAim::CameraAim(const std::shared_ptr<Transform> camera_transform) :
	m_camera_transform	(camera_transform),
	m_target_transform	(nullptr),
	m_aim_correct_dir	(v3d::GetZeroV()),
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

void CameraAim::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform		= target_transform;
	m_aim_correct_dir		= v3d::GetZeroV();
	m_horizontal_damping	= 0.0f;
	m_vertical_damping		= 0.0f;
	m_is_track				= true;
}

void CameraAim::AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& aim_correct_dir, const float horizontal_damping, const float vertical_damping)
{
	m_target_transform		= target_transform;
	m_aim_correct_dir		= aim_correct_dir;
	m_horizontal_damping	= horizontal_damping;
	m_vertical_damping		= vertical_damping;
	m_is_track				= true;
}

void CameraAim::DetachTarget()
{
	m_target_transform	= nullptr;
	m_is_track			= false;
}

void CameraAim::SetScreen(const Vector2D<float>& screen)
{
	m_screen = screen;

	std::clamp(m_screen.x, 0.0f, 1.0f);
	std::clamp(m_screen.y, 0.0f, 1.0f);
}

void CameraAim::SetDeadZone(const Vector2D<float>& dead_zone)
{
	m_dead_zone = dead_zone;

	std::clamp(m_dead_zone.x, 0.0f, 1.0f);
	std::clamp(m_dead_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetSoftZone(const Vector2D<float>& soft_zone)
{
	m_soft_zone = soft_zone;

	std::clamp(m_soft_zone.x, 0.0f, 1.0f);
	std::clamp(m_soft_zone.y, 0.0f, 1.0f);
}

void CameraAim::SetBias(const Vector2D<float>& bias)
{
	m_bias = bias;

	std::clamp(m_bias.x, -0.5f, 0.5f);
	std::clamp(m_bias.y, -0.5f, 0.5f);
}
