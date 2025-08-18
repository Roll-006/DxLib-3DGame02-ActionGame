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

	//const auto p		= m_transform->GetPos    (CoordinateKind::kWorld);
	//const auto forward	= m_transform->GetForward(CoordinateKind::kWorld);
	//const auto up		= m_transform->GetUp     (CoordinateKind::kWorld);
	//const auto right	= m_transform->GetRight  (CoordinateKind::kWorld);

	//DrawSphere3D  (p,					  5, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	//DrawCylinder3D(p, p + forward	* 10, 1, 8, GetColor(  0,   0, 255), GetColor(	0,   0, 255), TRUE);
	//DrawCylinder3D(p, p + up		* 10, 1, 8, GetColor(  0, 255,   0), GetColor(  0, 255,   0), TRUE);
	//DrawCylinder3D(p, p + right		* 10, 1, 8, GetColor(255,   0,   0), GetColor(255,   0,   0), TRUE);
}
