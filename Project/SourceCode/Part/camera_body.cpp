#include "camera_body.hpp"

CameraBody::CameraBody(const std::shared_ptr<Transform> camera_transform) :
	m_camera_transform	(camera_transform),
	m_target_transform	(nullptr),
	m_target_correct_pos(v3d::GetZeroV()),
	m_camera_correct_dir(v3d::GetZeroV()),
	m_distance_to_target(0.0f),
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


#pragma region Attach / Detach
void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& target_correct_pos)
{
	m_target_transform		= target_transform;
	m_target_correct_pos	= target_correct_pos;
	m_is_track				= true;
}

void CameraBody::DetachTarget()
{
	m_target_transform		= nullptr;
	m_is_track				= false;
}
#pragma endregion


#pragma region Getter
//VECTOR CameraBody::GetCameraPos() const
//{
//	// FIXME : 上家回転を行った際に位置がずれる
//	VECTOR camera_pos = m_target_transform->GetPos (CoordinateKind::kWorld);
//
//	// カメラの軸をもとに位置を決定
//	const auto camera_axes = m_camera_transform->GetAxes(CoordinateKind::kWorld);
//	camera_pos += camera_axes.x_axis * m_camera_correct_pos.x;
//	camera_pos += camera_axes.y_axis * m_camera_correct_pos.y;
//	camera_pos += camera_axes.z_axis * m_camera_correct_pos.z;
//
//	return camera_pos;
//}
#pragma endregion


void CameraBody::CalcPos()
{
	//m_camera_transform->SetPos(CoordinateKind::kWorld, GetCameraPos());
}
