#pragma once
#include "../Base/weapon_base.hpp"

class KnifeBase abstract : public WeaponBase
{
public:
	KnifeBase(const std::string& name, const HolsterKind holster_kind);
	virtual ~KnifeBase() = default;

protected:
	void CreateAttackTrigger(const VECTOR& trigger_offset_pos, const float trigger_radius);
	void CalcAttackTriggerPos();

private:
	VECTOR m_trigger_offset_pos;
	float  m_trigger_radius;
};
