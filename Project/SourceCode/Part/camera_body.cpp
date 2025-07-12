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


#pragma region Attach / Detach
void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform)
{
	m_target_transform		= target_transform;
	m_is_track				= true;
}

void CameraBody::AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& camera_correct_pos)
{
	m_target_transform		= target_transform;
	m_camera_correct_pos	= camera_correct_pos;
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
	//const auto target_world_mat		= MGetTranslate(m_target_transform->GetPos(CoordinateKind::kWorld));
	//const auto target_local_pos		= m_target_transform->GetPos   (CoordinateKind::kLocal);
	//const auto target_world_scale	= m_target_transform->GetScale (CoordinateKind::kWorld);
	//const auto camera_correct_pos	= m_camera_correct_dir * m_distance_to_target;

	//// スケールを打ち消す逆数
	//const VECTOR reciprocal
	//{
	//	1.0f / target_world_scale.x,
	//	1.0f / target_world_scale.y,
	//	1.0f / target_world_scale.z
	//};

	//return target_local_pos + VTransformSR(camera_correct_pos, target_world_mat) * reciprocal;


	const auto target_pos  = m_target_transform->GetPos (CoordinateKind::kWorld);
	const auto camera_axes = m_camera_transform->GetAxes(CoordinateKind::kWorld);
	VECTOR pos = target_pos;

	// カメラから見て位置を決定
	pos += camera_axes.x_axis * m_camera_correct_pos.x;
	pos += camera_axes.y_axis * m_camera_correct_pos.y;
	pos += camera_axes.z_axis * m_camera_correct_pos.z;

	return pos;
}
