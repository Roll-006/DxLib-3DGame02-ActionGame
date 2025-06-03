#pragma once
#include "obj_base.hpp"
#include "../Part/modeler.hpp"

class WeaponBase abstract : public ObjBase
{
public:
	WeaponBase(const std::string& name, const std::string& file_path) :
		ObjBase			(name, ObjTag.WEAPON),
		m_modeler		(std::make_shared<Modeler>(GetTransform(), file_path, false)),
		m_owner_modeler	(nullptr)
	{ }

	virtual ~WeaponBase() = default;

	virtual void TrackOwner() abstract;

	void AttachOwner(const std::shared_ptr<Modeler> owner_modeler)
	{
		if (!m_owner_modeler) { m_owner_modeler = owner_modeler; }
	}
	void DetachOwner()
	{
		if (m_owner_modeler) { m_owner_modeler = nullptr; }
	}

protected:
	std::shared_ptr<Modeler> m_modeler;
	std::shared_ptr<Modeler> m_owner_modeler;	// 武器の持ち主であるオブジェクトのモデラー
};
