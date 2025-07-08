#include "main_camera.hpp"
#include "../Manager/command_handler.hpp"

MainCamera::MainCamera() : 
	PhysicalObjBase(ObjName.MAIN_CAMERA, ObjTag.CAMERA, MassKind::kLight)
{

}

MainCamera::~MainCamera()
{
	// ˆ—‚È‚µ
}

void MainCamera::Init()
{

}

void MainCamera::Update()
{
	if (!IsActive()) { return; }
}

void MainCamera::LateUpdate()
{
	if (!IsActive()) { return; }
}

void MainCamera::Draw() const
{
	if (!IsActive()) { return; }
}

void MainCamera::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:
		if (hit_collider_pair.intersection)
		{
			m_transform->SetPos(CoordinateKind::kWorld, *hit_collider_pair.intersection);
			SetAim();
		}
		break;

	default:
		break;
	}
}

void MainCamera::SetAim()
{
	const VECTOR pos		= m_transform->GetPos(CoordinateKind::kWorld);
	const VECTOR target_pos	= pos + m_transform->GetForward(CoordinateKind::kWorld);

	SetCameraPositionAndTarget_UpVecY(pos, target_pos);
}
