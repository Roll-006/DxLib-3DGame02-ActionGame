#include "knife.hpp"

Knife::Knife() : 
	KnifeBase(ObjName.KNIFE, HolsterKind::kKnife)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.KNIFE, kModelBasicAngle);

	SetModelHandle(m_modeler->GetModelHandle());

	SetOffset(kHoldOffsetPos,   kHoldOffsetAngle,   kHoldOffsetScale,
			  kAttachOffsetPos, kAttachOffsetAngle, kAttachOffsetScale);

	CreateAttackTrigger(kTriggerOffsetPos, kTriggerRadius);
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

	TrackOwnerHand();
	CalcAttackTriggerPos();
}

void Knife::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	for (const auto& collider : m_collider)
	{
		const auto shape = collider->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
}

void Knife::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
