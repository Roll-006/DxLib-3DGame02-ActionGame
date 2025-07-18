#include "main_camera.hpp"
#include "../Command/command_handler.hpp"

MainCamera::MainCamera() : 
	PhysicalObjBase(ObjName.MAIN_CAMERA, ObjTag.CAMERA, MassKind::kLight)
{
	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
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

	SetAim();
}

void MainCamera::LateUpdate()
{
	if (!IsActive()) { return; }

	SetAim();
	CalcRayPos();
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

void MainCamera::CalcRayPos()
{
	// Œõü‚ÌÀ•W‚ğŒvZ
	//auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	//ray->SetBeginPos(GetLookPos(), true);
	//ray->SetEndPos	(m_transform->GetPos(CoordinateKind::kWorld), true);
}
