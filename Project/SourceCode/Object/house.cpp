#include "house.hpp"

House::House() :
	PhysicalObjBase	(ObjName.HOUSE, ObjTag.BUILDING, MassKind::kStatic),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.HOUSE_01, kBasicAngle, kBasicScale))
{
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_transform->SetPos(CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(kPos + kCollisionAreaOffset, kCollisionAreaRadius), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_modeler->GetModelHandle(), this));
}

House::~House()
{

}

void House::Init()
{

}

void House::Update()
{
	if (!IsActive()) { return; }
}

void House::LateUpdate()
{
	if (!IsActive()) { return; }
}

void House::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawColliders();
}

void House::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void House::AddToObjManager()
{
	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
	PhysicsManager	::GetInstance()->AddPhysicalObj	(std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this()));
}

void House::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

float House::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}
