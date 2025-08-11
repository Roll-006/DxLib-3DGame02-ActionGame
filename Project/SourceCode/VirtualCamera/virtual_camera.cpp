#include "virtual_camera.hpp"

VirtualCamera::VirtualCamera(const BlendActivationPolicyKind blend_policy_kind) :
	VirtualCameraBase(ObjName.NORMAL_VIRTUAL_CAMERA, VirtualCameraKind::kNormal, blend_policy_kind)
{

}

VirtualCamera::~VirtualCamera()
{

}

void VirtualCamera::Init()
{

}

void VirtualCamera::Update()
{
	if (!IsActive()) { return; }
}

void VirtualCamera::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcTransform();
}

void VirtualCamera::DrawToShadowMap() const
{
	if (!IsActive()) { return; }
}

void VirtualCamera::Draw() const
{
	if (!IsActive()) { return; }
}
