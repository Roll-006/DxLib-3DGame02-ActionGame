#include "scope_virtual_camera.hpp"

ScopeVirtualCamera::ScopeVirtualCamera() : 
	VirtualCameraBase(ObjName.SCOPE_CAMERA, VirtualCameraKind::kNormal)
{
	m_priority = kPriority;
	m_active_scene_kind.emplace_back(SceneKind::kPlay);

	m_aim->SetAimCorrect(VGet(0.0f, 20.0f, 0.0f));
}

ScopeVirtualCamera::~ScopeVirtualCamera()
{

}

void ScopeVirtualCamera::Init()
{

}

void ScopeVirtualCamera::Update()
{
	if (!IsActive()) { return; }

	m_aim ->SetRot(m_target_transform->GetRotMatrix(CoordinateKind::kWorld));
	m_body->SetCameraCorrectDir(-m_transform->GetForward(CoordinateKind::kWorld));
	m_body->SetDistanceToTarget(-10.0f);
	m_body->SetCameraPos(m_aim->GetAimPos() + m_body->GetCameraCorrectDir() * m_body->GetDistanceToTarget());
}

void ScopeVirtualCamera::LateUpdate()
{
	if (!IsActive()) { return; }

	m_body->SetCameraCorrectDir(-m_transform->GetForward(CoordinateKind::kWorld));
	m_body->SetDistanceToTarget(-10.0f);
	m_body->SetCameraPos(m_aim->GetAimPos() + m_body->GetCameraCorrectDir() * m_body->GetDistanceToTarget());
}

void ScopeVirtualCamera::Draw() const
{
	if (!IsActive()) { return; }
}
