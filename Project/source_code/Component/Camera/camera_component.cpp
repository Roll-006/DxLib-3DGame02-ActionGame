#include "camera_component.hpp"

CameraComponent::CameraComponent(GameObj* owner_obj):
	ComponentBase	(owner_obj),
	m_transform		(nullptr),
	m_tracker		(nullptr)
{
	SetCameraNearFar(kNear, kFar);
	SetupCamera_Perspective(kFOV * math::kDegreesToRadian);

	m_transform = m_owner_obj->GetComponent<TransformComponent>();
	m_tracker	= m_owner_obj->GetComponent<TrackerComponent>();
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Init()
{

}

void CameraComponent::Update()
{

}

void CameraComponent::LateUpdate()
{
	SetCameraPositionAndTarget_UpVecY(m_transform->GetPos(), m_tracker->GetCurrentTargetPos());
}

void CameraComponent::Draw()const
{
	DrawLine3D(VGet(0, 0, 0), VGet(10000, 0, 0), 0xff0000);
	DrawLine3D(VGet(0, 0, 0), VGet(0, 10000, 0), 0x00ff22);
	DrawLine3D(VGet(0, 0, 0), VGet(0, 0, 10000), 0x0077ff);
}
