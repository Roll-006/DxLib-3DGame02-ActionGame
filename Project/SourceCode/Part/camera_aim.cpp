#include "camera_aim.hpp"

CameraAim::CameraAim(const std::shared_ptr<Transform> owner_transform) :
	m_owner_transform		(owner_transform),
	m_target_transform		(nullptr),
	m_rot_matrix			(MGetIdent()),
	m_destination_aim_pos	(v3d::GetZeroV()),
	m_current_aim_pos		(v3d::GetZeroV()),
	m_aim_correct			(v3d::GetZeroV()),
	m_horizontal_damping	(0.0f),
	m_vertical_damping		(0.0f),
	m_screen				(0.5f, 0.5f),
	m_dead_zone				(v2d::GetZeroV<Vector2D<float>>()),
	m_soft_zone				(v2d::GetZeroV<Vector2D<float>>()),
	m_bias					(v2d::GetZeroV<Vector2D<float>>()),
	m_is_track				(false)
{

}

CameraAim::~CameraAim()
{

}

void CameraAim::CalcAimPos()
{
	m_destination_aim_pos = m_target_transform->GetPos(CoordinateKind::kWorld);

	// カメラの軸をもとに位置を修正
	const auto target_axes = m_target_transform->GetAxes(CoordinateKind::kWorld);
	m_destination_aim_pos += target_axes.x_axis * m_aim_correct.x;
	m_destination_aim_pos += target_axes.y_axis * m_aim_correct.y;
	m_destination_aim_pos += target_axes.z_axis * m_aim_correct.z;

	DrawSphere3D(m_destination_aim_pos, 3, 8, 0xffffff, 0xffffff, TRUE);
}

void CameraAim::CalcDampedAimPos()
{
	const auto owner_axes	= m_owner_transform->GetAxes(CoordinateKind::kWorld);
	const auto distance		= m_destination_aim_pos - m_current_aim_pos;

	// カメラの軸に分解（内積）
	float forward_dot		= VDot(distance, owner_axes.z_axis);
	float right_dot			= VDot(distance, owner_axes.x_axis);
	float up_dot			= VDot(distance, owner_axes.y_axis);

	// 各軸の移動量に対して減衰を適用
	float damped_forward	= math::GetDampedValue(0.0f, forward_dot, 0.0f);
	float damped_right		= math::GetDampedValue(0.0f, right_dot,   m_horizontal_damping);
	float damped_up			= math::GetDampedValue(0.0f, up_dot,      m_vertical_damping);

	// 4. 減衰移動ベクトルを再合成
	const VECTOR damped_move =
		owner_axes.z_axis * damped_forward +
		owner_axes.x_axis * damped_right +
		owner_axes.y_axis * damped_up;

	m_current_aim_pos += damped_move;

	DrawSphere3D(m_current_aim_pos, 3, 8, 0xffffff, 0xffffff, TRUE);
}

void CameraAim::CalcRot()
{
	CalcAimPos();

	// 目的とする回転行列を取得
	// Z軸回転は無効化する
	const auto forward	= v3d::GetNormalizedV(m_destination_aim_pos - m_owner_transform->GetPos(CoordinateKind::kWorld));
	const auto right	= -math::GetNormalVector(forward, axis::GetWorldYAxis());
	const auto up		= math::GetNormalVector(forward, right);
	const auto axes		= Axes(right, up, forward);
	m_rot_matrix = math::ConvertAxesToXYZRotMatrix(axes);

	m_owner_transform->SetRot(CoordinateKind::kWorld, m_rot_matrix);
}

void CameraAim::CalcDampedRot()
{
	CalcDampedAimPos();

	// 目的とする回転行列を取得
	// Z軸回転は無効化する
	const auto forward	= v3d::GetNormalizedV(m_current_aim_pos - m_owner_transform->GetPos(CoordinateKind::kWorld));
	const auto right	= -math::GetNormalVector(forward, axis::GetWorldYAxis());
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
