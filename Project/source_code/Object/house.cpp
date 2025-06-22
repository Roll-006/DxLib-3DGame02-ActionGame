#include "house.hpp"

House::House() :
	PhysicalObjBase(ObjName.HOUSE, ObjTag.BUILDING, MassKind::kStatic)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.HOUSE_01);

	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, kPos);
}

House::~House()
{

}

void House::Init()
{

}

void House::Update()
{

}

void House::Draw() const
{
	m_modeler->Draw();
}

void House::OnCollide(const ColliderPairData& hit_collider_pair)
{

}
