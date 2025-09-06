#include "virtual_camera.hpp"

VirtualCamera::VirtualCamera(const std::string& name, const BlendActivationPolicyKind blend_policy_kind) :
	VirtualCameraBase(name, blend_policy_kind)
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
