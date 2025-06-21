#include "ground.hpp"

Ground::Ground() : 
	PhysicalObjBase(ObjName.GROUND, ObjTag.GROUND, MassKind::kVeryHeavy)
{
	MakeModel(ModelPath.GROUND_01, VGet(-90.0f * math::kDegreesToRadian, 0.0f, 0.0f));

	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, kPos);
}

Ground::~Ground()
{

}

void Ground::Init()
{

}

void Ground::Update()
{

}

void Ground::Draw() const
{
	m_modeler->Draw();
}

void Ground::OnCollide(const PhysicalObjBase& check_hit_obj)
{

}
