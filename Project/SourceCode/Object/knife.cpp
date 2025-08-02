#include "knife.hpp"

Knife::Knife() : 
	KnifeBase(ObjName.KNIFE, HolsterKind::kKnife)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.KNIFE, kModelBasicAngle);

	SetModelHandle(m_modeler->GetModelHandle());
	SetOffset(kOffsetPos, kOffsetAngle, kOffsetScale);
}

Knife::~Knife()
{

}

void Knife::Init()
{

}

void Knife::Update()
{
	if (!IsActive()) { return; }
}

void Knife::LateUpdate()
{
	if (!IsActive()) { return; }

	TrackOwner();
}

void Knife::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void Knife::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
