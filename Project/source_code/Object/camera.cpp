#include "camera.hpp"

Camera::Camera() : 
	ObjBase				(ObjName.CAMERA, ObjTag.CAMERA, VGet(0.0f, 0.0f, -500.0f)),
	m_target_transform	(nullptr),
	m_move_speed		(0.0f),
	m_distance_to_target(0.0f)
{
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegreesToRadian);
}

Camera::~Camera()
{
	// ˆ—‚È‚µ
}

void Camera::Init()
{

}

void Camera::Update()
{
	SetCameraPositionAndTarget_UpVecY(GetTransform()->GetPos(), m_target_transform->GetPos());
}

void Camera::Draw()const
{
	DrawLine3D(VGet(0, 0, 0), VGet(10000,	  0,	 0), 0xff0000);
	DrawLine3D(VGet(0, 0, 0), VGet(	   0, 10000,	 0), 0x00ff22);
	DrawLine3D(VGet(0, 0, 0), VGet(    0,	  0, 10000), 0x0077ff);
}

void Camera::AttachTarget(const std::shared_ptr<ObjBase> obj)
{
	m_target_transform = obj->GetTransform();
}

void Camera::AttachTarget(const std::string& obj_name)
{
	auto target_obj = ObjManager::GetInstance()->GetObj(obj_name);
	m_target_transform = target_obj->GetTransform();
}

void Camera::DetachTarget()
{
	m_target_transform = nullptr;
}
