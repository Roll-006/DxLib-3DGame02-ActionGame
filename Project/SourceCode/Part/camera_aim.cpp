#include "camera_aim.hpp"

CameraAim::CameraAim(const std::shared_ptr<Transform> owner_transform) :
	m_owner_transform	(owner_transform),
	m_target_transform	(nullptr),
	//m_target_correct_pos(v3d::GetZeroV()),
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

void CameraAim::CalcAimPos()
{
	m_aim_pos = m_target_transform->GetPos(CoordinateKind::kWorld);

	// カメラの軸をもとに位置を修正
	const auto target_axes = m_target_transform->GetAxes(CoordinateKind::kWorld);
	m_aim_pos += target_axes.x_axis * m_aim_correct.x;
	m_aim_pos += target_axes.y_axis * m_aim_correct.y;
	m_aim_pos += target_axes.z_axis * m_aim_correct.z;

	DrawSphere3D(m_aim_pos, 3, 8, 0xffffff, 0xffffff, TRUE);
}

void CameraAim::CalcRot()
{
	// Z軸回転は無効化する
	const auto forward	= v3d::GetNormalizedV(m_aim_pos - m_owner_transform->GetPos(CoordinateKind::kWorld));
	const auto right	= math::GetNormalVector(forward, axis::GetWorldYAxis());
	const auto up		= math::GetNormalVector(forward, right);
	const auto axes		= Axes(right, up, forward);
	m_rot_matrix = math::ConvertAxesToXYZRotMatrix(axes);

	m_owner_transform->SetRot(CoordinateKind::kWorld, m_rot_matrix);
}


#pragma region Attach / Detach
void CameraAim::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform		= target_transform;
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
	m_owner_transform->SetRot(CoordinateKind::kWorld, rot_matrix);
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
