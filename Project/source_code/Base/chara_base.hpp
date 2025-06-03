#pragma once
#include "physical_obj_base.hpp"
#include "gun_base.hpp"

#include "../Part/modeler.hpp"
#include "../Part/animator.hpp"

class CharaBase abstract : public PhysicalObjBase
{
public:
	CharaBase(const std::string& name, const std::string& tag, const std::string& file_path, MassKind mass_level_kind) :
		PhysicalObjBase		(name, tag, mass_level_kind),
		m_modeler			(std::make_shared<Modeler>(GetTransform(), file_path, true)),
		m_animator			(std::make_shared<Animator>(m_modeler)),
		m_current_attach_gun(nullptr)
	{ }

	virtual ~CharaBase() = default;

	virtual void ChangeAnimState() abstract;

	/// @brief e‚ÌŠ“o˜^ 
	void AddGun(const std::shared_ptr<GunBase> gun)
	{
		if (!m_guns.count(gun->GetGunKind())) { m_guns[gun->GetGunKind()] = gun; }
	}
	/// @brief e‚ÌŠ“o˜^‚ğ‰ğœ
	void RemoveGun(const GunKind gun_kind)
	{
		m_guns.erase(gun_kind);
	}

	/// @brief e‚ğƒAƒ^ƒbƒ`(‘•”õ)‚·‚é
	void AttachGun(const GunKind gun_kind)
	{
		if (m_guns.count(gun_kind))
		{
			m_current_attach_gun = m_guns.at(gun_kind);
			m_current_attach_gun->AttachOwner(m_modeler);
		}
	}
	/// @brief e‚ğƒfƒ^ƒbƒ`(‘•”õ‰ğœ)‚·‚é
	void DetachGun(const GunKind gun_kind)
	{
		m_current_attach_gun = nullptr;
	}

	[[nodiscard]] const std::shared_ptr<Modeler> GetModeler()const { return m_modeler; }

protected:
	std::shared_ptr<Modeler>  m_modeler;
	std::shared_ptr<Animator> m_animator;
	std::unordered_map<GunKind, std::shared_ptr<GunBase>> m_guns;	// “o˜^(Š)‚µ‚Ä‚¢‚ée
	std::shared_ptr<GunBase>  m_current_attach_gun;					// Œ»İƒAƒ^ƒbƒ`(‘•”õ)‚µ‚Ä‚¢‚ée
};
