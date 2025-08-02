#include "knife_base.hpp"

KnifeBase::KnifeBase(const std::string& name, const HolsterKind holster_kind) :
	WeaponBase(name, WeaponKind::kKnife, holster_kind)
{

}
