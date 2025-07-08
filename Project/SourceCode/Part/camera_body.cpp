#include "camera_body.hpp"

CameraBody::CameraBody(const std::shared_ptr<Transform> camera_transform) :
	m_camera_transform	(camera_transform),
	m_target_transform	(nullptr),
	m_track_correct_pos	(v3d::GetZeroV()),
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

void CameraBody::Update()
{
	CalcCameraPos();
}

void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform		= target_transform;
	m_track_correct_pos		= v3d::GetZeroV();
	m_damping				= v3d::GetZeroV();
	m_damping_yaw			= 0.0f;
	m_is_track				= true;
}

void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& follow_correct_pos, const VECTOR& damping, const float damping_yaw)
{
	m_target_transform		= target_transform;
	m_track_correct_pos		= follow_correct_pos;
	m_damping				= damping;
	m_damping_yaw			= damping_yaw;
	m_is_track				= true;
}

void CameraBody::DetachTarget()
{
	m_target_transform		= nullptr;
	m_is_track				= false;
}

void CameraBody::CalcCameraPos()
{
	const VECTOR target_pos = m_target_transform->GetPos(CoordinateKind::kWorld);
	m_camera_transform->SetPos(CoordinateKind::kWorld, target_pos + m_track_correct_pos);
}
