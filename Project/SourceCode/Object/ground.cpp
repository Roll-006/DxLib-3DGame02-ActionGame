#include "ground.hpp"

Ground::Ground() : 
	PhysicalObjBase	(ObjName.GROUND, ObjTag.GROUND, MassKind::kStatic),
	m_modeler		(std::make_shared<Modeler>(m_transform, ModelPath.GROUND_01, kBasicAngle, kBasicScale))
{	
	SetModelHandle(m_modeler->GetModelHandle());

	m_transform->SetPos (CoordinateKind::kWorld, kPos);
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

void Ground::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void Ground::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void Ground::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
