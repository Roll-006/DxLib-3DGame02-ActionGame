#pragma once
#include "../Base/weapon_base.hpp"

class KnifeBase abstract : public WeaponBase
{
public:
	KnifeBase(const std::string& name, const HolsterKind holster_kind);
	virtual ~KnifeBase() = default;
};
