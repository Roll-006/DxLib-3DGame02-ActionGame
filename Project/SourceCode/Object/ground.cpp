#include "ground.hpp"

Ground::Ground() : 
	PhysicalObjBase	(ObjName.GROUND, ObjTag.GROUND),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.GROUND, kBasicAngle, kBasicScale))
{	
	mass_kind = MassKind::kStatic;

	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_transform->SetPos(CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_modeler->GetModelHandle(), this));
}

Ground::~Ground()
{

}

void Ground::Init()
{

}

void Ground::Update()
{
	if (!IsActive()) { return; }
}

void Ground::LateUpdate()
{
	if (!IsActive()) { return; }
}

void Ground::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//DrawColliders();
}

void Ground::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void Ground::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void Ground::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

float Ground::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
