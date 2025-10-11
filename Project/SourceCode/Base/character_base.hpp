#pragma once
#include "physical_obj_base.hpp"

#include "../Interface/i_state_controller.hpp"
#include "../Part/character_collider_creator.hpp"
#include "../Part/health.hpp"
#include "../Part/modeler.hpp"
#include "animator_base.hpp"

#include "gun_base.hpp"
#include "../Kind/health_part_kind.hpp"

class CharacterBase abstract : public PhysicalObjBase
{
public:
	CharacterBase(const std::string& name, const std::string& tag, const MassKind mass_level_kind);
	virtual ~CharacterBase() = default;

	void AddToObjManager()		override;
	void RemoveToObjManager()	override;

	void CalcCorrectMoveDir();

	/// @brief ‹­§“I‚É–³“Gó‘Ô‚É‚·‚é
	void ActivateInvincibleForcibly()	{ m_is_invincible = true; }
	/// @brief ‹­§“I‚É–³“Gó‘Ô‚ğ‰ğœ‚·‚é
	void DeactivateInvincibleForcibly() { m_is_invincible = false; }

	virtual void OnDamage(const HealthPartKind part_kind, const float damage) abstract;

	#pragma region Getter
	[[nodiscard]] bool							IsInvincible()				const		{ return m_is_invincible; }
	[[nodiscard]] std::shared_ptr<Modeler>		GetModeler()				const		{ return m_modeler; }
	[[nodiscard]] std::shared_ptr<AnimatorBase>	GetAnimator()				const		{ return m_animator; }
	[[nodiscard]] VECTOR						GetCurrentMoveDir()			const		{ return m_move_dir.at(TimeKind::kCurrent); }
	[[nodiscard]] VECTOR						GetCurrentLookDir()			const		{ return m_look_dir.at(TimeKind::kCurrent); }
	//[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentHeldWeapon()		const { return m_current_held_weapon; }
	//[[nodiscard]] WeaponKind					GetCurrentHeldWeaponKind();
	//[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentAttachWeapon		(const HolsterKind holster_kind)	const;
	//[[nodiscard]] WeaponKind					GetCurrentAttachWeaponKind	(const HolsterKind holster_kind)	const;
	[[nodiscard]] std::shared_ptr<Gauge>&		GetHealth(const HealthPartKind kind)	{ return m_health.at(kind); }
	#pragma endregion

protected:
	/// @brief Œ©‚Ä‚¢‚é•ûŒü‚ğ‰ñ“]‚É“K—p‚·‚é
	void ApplyLookDirToRot(const VECTOR& look_dir);

	void CalcMoveDir();

	void JudgeInvincible();

protected:
	std::shared_ptr<Modeler>					m_modeler;
	std::shared_ptr<AnimatorBase>				m_animator;
	std::shared_ptr<CharacterColliderCreator>	m_collider_creator;

	float										m_move_speed;
	std::unordered_map<TimeKind, VECTOR>		m_move_dir;					// ˆÚ“®•ûŒü(WARNING : ’·‚³‚Í0`1‚Ì”ÍˆÍ‚ğæ‚é)
	std::unordered_map<TimeKind, VECTOR>		m_look_dir;					// Œü‚¢‚Ä‚¢‚é•ûŒü
	float										m_move_dir_offset_speed;	// ˆÚ“®•ûŒü‚ğ•â³‚·‚é‘¬“x
	VECTOR										m_destination_pos;			// •â³æÀ•W

	std::shared_ptr<WeaponBase>											m_current_held_weapon;	// Œ»İè‚É‚Á‚Ä‚¢‚é•Ší
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>>		m_attach_weapons;		// ‘•’…‚µ‚Ä‚¢‚é•Ší

	std::unordered_map<HealthPartKind, std::shared_ptr<Gauge>>	m_health;

	float m_invincible_time;
	float m_invincible_timer;
	bool  m_is_invincible;
};
