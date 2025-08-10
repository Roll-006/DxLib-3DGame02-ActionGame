#include "virtual_camera.hpp"

VirtualCamera::VirtualCamera() : 
	VirtualCameraBase(ObjName.NORMAL_VIRTUAL_CAMERA, VirtualCameraKind::kNormal)
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
