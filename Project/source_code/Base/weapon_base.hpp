#pragma once
#include "obj_base.hpp"

#include "../Part/modeler.hpp"

class WeaponBase : public ObjBase
{
public:
	WeaponBase(const std::string& name, const std::string& file_path) :
		ObjBase		(name, ObjTag.WEAPON),
		m_modeler	(std::make_shared<Modeler>(GetTransform(), file_path))
	{ }

	virtual ~WeaponBase() = default;

protected:
	std::shared_ptr<Modeler>  m_modeler;
};
