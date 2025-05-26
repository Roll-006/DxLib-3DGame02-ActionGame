#pragma once
#include "physical_obj_base.hpp"

class CharaBase abstract : public PhysicalObjBase
{
	CharaBase(const std::string& name, const std::string& tag, MassKind mass_level_kind) :
		PhysicalObjBase(name, tag, mass_level_kind)
	{ }

	virtual ~CharaBase() = default;
};