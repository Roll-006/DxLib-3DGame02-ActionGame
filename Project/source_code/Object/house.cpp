#include "house.hpp"

House::House() :
	PhysicalObjBase(ObjName.HOUSE, ObjTag.BUILDING, MassKind::kStatic)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.HOUSE_01);

	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, kPos);

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_modeler->GetModelHandle(), this));
	m_modeler->ApplyMatrix();
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
}

void House::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
