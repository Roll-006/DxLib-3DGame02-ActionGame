#pragma once
#include "physical_obj_base.hpp"

#include "../Part/character_collider_creator.hpp"
#include "../Part/hit_points.hpp"
#include "../Part/modeler.hpp"
#include "animator_base.hpp"

#include "gun_base.hpp"

class CharacterBase abstract : public PhysicalObjBase
{
public:
	CharacterBase(const std::string& name, const std::string& tag, const MassKind mass_level_kind);
	virtual ~CharacterBase() = default;

	void AddToObjManager() override;


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<Modeler>		GetModeler()					const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<AnimatorBase>	GetAnimator()					const { return m_animator; }
	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentHeldWeapon()			const { return m_current_held_weapon; }
	[[nodiscard]] WeaponKind					GetCurrentHeldWeaponKind();
	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentAttachWeapon		(const HolsterKind holster_kind) const;
	[[nodiscard]] WeaponKind					GetCurrentAttachWeaponKind	(const HolsterKind holster_kind) const;
	#pragma endregion


	#pragma region •Ší
	/// @brief •Ší‚ğè‚É‚½‚¹‚é
	template<obj_concepts::WeaponT WeaponObjT>
	void HoldWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		m_current_held_weapon = weapon;
		m_current_held_weapon->AttachOwner(m_modeler, GetName());
	}
	void HoldWeapon(const int obj_handle);

	/// @brief è‚É‚Á‚Ä‚¢‚é•Ší‚ğ•ú‚·
	void ReleaseWeapon();

	/// @brief •Ší‚ğ‘•’…‚·‚é
	template<obj_concepts::WeaponT WeaponObjT>
	void AttachWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		m_attach_weapons[weapon->GetHolsterKind()] = weapon;
		m_attach_weapons[weapon->GetHolsterKind()]->AttachOwner(m_modeler, GetName());
	}
	void AttachWeapon(const int obj_handle);

	/// @brief •Ší‚Ì’…’E‚·‚é
	template<obj_concepts::WeaponT WeaponObjT>
	void DetachWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		// ©g‚ª‘•’…‚³‚ê‚Ä‚¢‚ê‚Î’…’E‚·‚é
		if (m_attach_weapons.count(weapon->GetHolsterKind()))
		{
			if (m_attach_weapons[weapon->GetHolsterKind()] == weapon)
			{
				m_attach_weapons[weapon->GetHolsterKind()]->DetachOwner();
				m_attach_weapons[weapon->GetHolsterKind()] = nullptr;
			}
		}
	}
	void DetachWeapon(const HolsterKind holster_kind);
	#pragma endregion

protected:
	/// @brief Œ©‚Ä‚¢‚é•ûŒü‚ğ‰ñ“]‚É“K—p‚·‚é
	void ApplyLookDirToRot(const VECTOR& look_dir);

protected:
	std::shared_ptr<Modeler>		m_modeler;
	std::shared_ptr<AnimatorBase>	m_animator;
	std::shared_ptr<CharacterColliderCreator> m_collider_creator;

	std::shared_ptr<WeaponBase>		m_current_held_weapon;							// Œ»İè‚É‚Á‚Ä‚¢‚é•Ší
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>> m_attach_weapons;	// ‘•’…‚µ‚Ä‚¢‚é•Ší

	std::unordered_map<HitPointsPartKind, std::shared_ptr<HitPoints>> m_hit_points;
};
