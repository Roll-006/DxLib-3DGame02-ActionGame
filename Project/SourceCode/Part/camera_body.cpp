#include "camera_body.hpp"

CameraBody::CameraBody(const std::shared_ptr<Transform> camera_transform) :
	m_camera_transform	(camera_transform),
	m_target_transform	(nullptr),
	m_camera_correct_pos(v3d::GetZeroV()),
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

void CameraBody::CalcCameraPos()
{
	m_camera_transform->SetPos(CoordinateKind::kWorld, GetCameraPos());
}


void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform		= target_transform;
	//m_camera_correct_pos	= v3d::GetZeroV();
	//m_damping				= v3d::GetZeroV();
	//m_damping_yaw			= 0.0f;
	m_is_track				= true;
}


#pragma region Attach / Detach
void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& camera_correct_pos, const VECTOR& damping, const float damping_yaw)
{
	m_target_transform		= target_transform;
	m_camera_correct_pos	= camera_correct_pos;
	m_damping				= damping;
	m_damping_yaw			= damping_yaw;
	m_is_track				= true;
}

void CameraBody::DetachTarget()
{
	m_target_transform		= nullptr;
	m_is_track				= false;
}
#pragma endregion


VECTOR CameraBody::GetCameraPos() const
{
	const auto target_world_mat = m_target_transform->GetMatrix(CoordinateKind::kWorld);
	const auto target_local_pos = m_target_transform->GetPos   (CoordinateKind::kLocal);

	// TODO : スケールを打ち消す必要あり
	return target_local_pos + VTransformSR(m_camera_correct_pos, target_world_mat);
}
