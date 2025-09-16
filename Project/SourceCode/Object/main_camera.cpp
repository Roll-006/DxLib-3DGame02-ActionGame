#include "main_camera.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Command/command_handler.hpp"

MainCamera::MainCamera() : 
	PhysicalObjBase(ObjName.MAIN_CAMERA, ObjTag.CAMERA, MassKind::kLight)
{
	//AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
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

	SetAim();
	CalcRayPos();
}

void MainCamera::DrawToShadowMap() const
{

}

void MainCamera::Draw() const
{
	if (!IsActive()) { return; }

	//const auto p		= m_transform->GetPos    (CoordinateKind::kWorld);
	//const auto forward	= m_transform->GetForward(CoordinateKind::kWorld);
	//const auto up		= m_transform->GetUp     (CoordinateKind::kWorld);
	//const auto right	= m_transform->GetRight  (CoordinateKind::kWorld);

	//DrawSphere3D  (p,					  5, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), TRUE);
	//DrawCylinder3D(p, p + forward	* 10, 1, 8, GetColor(  0,   0, 255), GetColor(  0,   0, 255), TRUE);
	//DrawCylinder3D(p, p + up		* 10, 1, 8, GetColor(  0, 255,   0), GetColor(  0, 255,   0), TRUE);
	//DrawCylinder3D(p, p + right		* 10, 1, 8, GetColor(255,   0,   0), GetColor(255,   0,   0), TRUE);

	//DrawFormatString(600, 0, 0xffffff, "%f, %f, %f", p.x, p.y, p.z);
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

void MainCamera::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj				(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddIgnoreObjGravity(this->GetObjHandle());

	CinemachineBrain::GetInstance()->SetMainCamera(std::static_pointer_cast<MainCamera>(shared_from_this()));
}

void MainCamera::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	PhysicsManager	::GetInstance()->RemoveIgnoreObjGravity	(obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj		(obj_handle);
	CollisionManager::GetInstance()->RemoveCollideObj		(obj_handle);
	ObjManager		::GetInstance()->RemoveObj				(obj_handle);
}

float MainCamera::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera);
}

void MainCamera::SetAim()
{
	const VECTOR pos		= m_transform->GetPos(CoordinateKind::kWorld);
	const VECTOR target_pos	= pos + m_transform->GetForward(CoordinateKind::kWorld);

	SetCameraPositionAndTarget_UpVecY(pos, target_pos);
	//SetCameraPositionAndTarget_UpVecY(VGet(0, 300, 0), VGet(0, -1, 0));
}

void MainCamera::CalcRayPos()
{
	// Œõü‚ÌÀ•W‚ğŒvZ
	//auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	//ray->SetBeginPos(GetLookPos(), true);
	//ray->SetEndPos	(m_transform->GetPos(CoordinateKind::kWorld), true);
}
