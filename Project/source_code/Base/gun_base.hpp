#pragma once
#include "weapon_base.hpp"

class GunBase : public WeaponBase
{
public:
	GunBase(const std::string& name, const std::string& tag) :
		WeaponBase(name, tag)
	{ }

	virtual ~GunBase() = default;

private:

};
