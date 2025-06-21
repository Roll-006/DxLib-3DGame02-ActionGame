#include "house.hpp"

House::House() :
	PhysicalObjBase(ObjName.HOUSE, ObjTag.BUILDING, MassKind::kVeryHeavy)
{
	MakeModel(ModelPath.HOUSE_01);

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

void House::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}

