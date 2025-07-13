#include "scope_virtual_camera.hpp"

ScopeVirtualCamera::ScopeVirtualCamera() : 
	VirtualCameraBase(ObjName.SCOPE_CAMERA, VirtualCameraKind::kNormal)
{
	m_priority = kPriority;
	m_active_scene_kind.emplace_back(SceneKind::kPlay);

	m_body->SetCameraCorrectDir(VGet(0.0f, 0.0f, -200.0f));
}

ScopeVirtualCamera::~ScopeVirtualCamera()
{

}

void ScopeVirtualCamera::Init()
{

}

void ScopeVirtualCamera::Update()
{
	//// TODO : ’Ç”ö•û–@‚ðe‚©‚çƒvƒŒƒCƒ„[‚É•ÏX(U“®‚ð—}‚¦‚é‚½‚ß)
	//m_aim->SetRot(m_target_transform->GetRotMatrix(CoordinateKind::kWorld));
	//m_body->Update();
}

void ScopeVirtualCamera::LateUpdate()
{

}

void ScopeVirtualCamera::Draw() const
{

}
