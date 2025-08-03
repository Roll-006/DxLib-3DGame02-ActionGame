#include "knife_base.hpp"

KnifeBase::KnifeBase(const std::string& name, const HolsterKind holster_kind) :
	WeaponBase			(name, WeaponKind::kKnife, holster_kind),
	m_trigger_offset_pos(v3d::GetZeroV()),
	m_trigger_radius	(0.0f)
{

}

void KnifeBase::CreateAttackTrigger(const VECTOR& trigger_offset_pos, const float trigger_radius)
{
	m_trigger_offset_pos = trigger_offset_pos;

	AddCollider(std::make_shared<Collider>(ColliderKind::kAttackTrigger, std::make_shared<Sphere>(v3d::GetZeroV(), trigger_radius), this));
}

void KnifeBase::CalcAttackTriggerPos()
{
	const auto m	= m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto pos	= VTransform(m_trigger_offset_pos, m);

	std::dynamic_pointer_cast<Sphere>(GetCollider(ColliderKind::kAttackTrigger)->GetShape())->SetPos(pos);
}
