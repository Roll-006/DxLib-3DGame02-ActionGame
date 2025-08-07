#include "house.hpp"

House::House() :
	PhysicalObjBase	(ObjName.HOUSE, ObjTag.BUILDING, MassKind::kStatic),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.HOUSE_01, kBasicAngle, kBasicScale))
{
	SetModelHandle(m_modeler->GetModelHandle());

	m_transform->SetPos  (CoordinateKind::kWorld, kPos);
	m_modeler->ApplyMatrix();

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

void House::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void House::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
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
