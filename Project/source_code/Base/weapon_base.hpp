#pragma once
#include "obj_base.hpp"

class WeaponBase : public ObjBase
{
public:
	WeaponBase(const std::string& name, const std::string& tag) :
		ObjBase(name, tag)
	{ }

	virtual ~WeaponBase() = default;

private:

};
