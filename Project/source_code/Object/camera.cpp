#include "camera.hpp"

Camera::Camera() : 
	m_transform(std::make_shared<Transform>(VGet(0.0f, 0.0f, -500.0f)))
{
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegreesToRadian);
}

Camera::~Camera()
{

}

void Camera::Init()
{

}

void Camera::Update()
{
	SetCameraPositionAndTarget_UpVecY(m_transform->GetPos(), m_tracker->GetCurrentTargetPos());
}

void Camera::Draw()const
{
	DrawLine3D(VGet(0, 0, 0), VGet(10000, 0, 0), 0xff0000);
	DrawLine3D(VGet(0, 0, 0), VGet(0, 10000, 0), 0x00ff22);
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, 10000), 0x0077ff);
}
