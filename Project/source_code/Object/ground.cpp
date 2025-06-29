#include "ground.hpp"

Ground::Ground() : 
	PhysicalObjBase(ObjName.GROUND, ObjTag.GROUND, MassKind::kStatic)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.GROUND_01, VGet(-90.0f * math::kDegreesToRadian, 0.0f, 0.0f));

	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, kPos);

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, m_modeler->GetModelHandle(), this));
	m_modeler->ApplyMatrix();
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
}

void Ground::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
