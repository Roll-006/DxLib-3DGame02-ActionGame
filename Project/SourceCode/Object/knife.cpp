#include "knife.hpp"

Knife::Knife() : 
	KnifeBase(ObjName.KNIFE, HolsterKind::kKnife)
{
	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.KNIFE, kBasicAngle, kBasicScale);	
	SetColliderModelHandle(m_modeler->GetModelHandle());

	SetOffset(kHoldOffsetPos,   kHoldOffsetAngle,   kHoldOffsetScale,
			  kAttachOffsetPos, kAttachOffsetAngle, kAttachOffsetScale);

	CreateAttackTrigger(kTriggerOffsetPos, kTriggerRadius);

	m_power = kPower;
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

	//TrackOwnerHand();
	CalcAttackTriggerPos();
}

void Knife::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void Knife::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	for (const auto& collider : m_colliders)
	{
		const auto shape = collider.second->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
}

void Knife::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}
