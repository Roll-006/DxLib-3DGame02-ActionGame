#pragma once
#include "physical_obj_base.hpp"

#include "../Interface/i_state_controller.hpp"
#include "../Part/character_collider_creator.hpp"
#include "../Part/gauge.hpp"
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
	void AllowCalcLookDir() { m_is_calc_look_dir = true; }

	/// @brief 強制的に無敵状態にする
	void ActivateInvincibleForcibly()	{ m_is_invincible = true; }
	/// @brief 強制的に無敵状態を解除する
	void DeactivateInvincibleForcibly() { m_is_invincible = false; }

	virtual void OnDamage(const HealthPartKind part_kind, const float damage) abstract;

	#pragma region Getter
	[[nodiscard]] bool							IsInvincible()		const				{ return m_is_invincible; }
	[[nodiscard]] bool							IsAlive()			const				{ return m_health.at(HealthPartKind::kMain)->IsAlive(); }
	[[nodiscard]] std::shared_ptr<Modeler>		GetModeler()		const				{ return m_modeler; }
	[[nodiscard]] std::shared_ptr<AnimatorBase>	GetAnimator()		const				{ return m_animator; }
	[[nodiscard]] VECTOR						GetCurrentMoveDir()	const				{ return m_move_dir.at(TimeKind::kCurrent); }
	[[nodiscard]] VECTOR						GetCurrentLookDir()	const				{ return m_look_dir.at(TimeKind::kCurrent); }
	[[nodiscard]]
	[[nodiscard]] std::shared_ptr<Gauge>&		GetHealth(const HealthPartKind kind)	{ return m_health.at(kind); }
	#pragma endregion

protected:
	/// @brief 見ている方向を回転に適用する
	void ApplyLookDirToRot(const VECTOR& look_dir);

	void CalcMoveDir();
	void CalcLookDir();
	void CalcMoveVelocity();

	void JudgeInvincible();

protected:
	std::shared_ptr<Modeler>					m_modeler;
	std::shared_ptr<AnimatorBase>				m_animator;
	std::shared_ptr<CharacterColliderCreator>	m_collider_creator;

	std::unordered_map<TimeKind, VECTOR>		m_move_dir;					// 移動方向(WARNING : 長さは0～1の範囲を取る)
	std::unordered_map<TimeKind, VECTOR>		m_look_dir;					// 向いている方向
	VECTOR										m_destination_pos;			// 補正先座標
	float										m_move_speed;
	float										m_move_dir_offset_speed;	// 移動方向を補正する速度
	float										m_look_dir_offset_speed;	// 見る方向の補正速度
	bool										m_is_calc_look_dir;

	std::shared_ptr<WeaponBase>											m_current_held_weapon;	// 現在手に持っている武器
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>>		m_attach_weapons;		// 装着している武器

	std::unordered_map<HealthPartKind, std::shared_ptr<Gauge>>	m_health;

	float m_invincible_time;
	float m_invincible_timer;
	bool  m_is_invincible;

	friend void from_json(const nlohmann::json& data, CharacterBase& character_base);
	friend void to_json  (nlohmann::json& data, const CharacterBase& character_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, CharacterBase& character_base)
{
	from_json(data, static_cast<PhysicalObjBase&>(character_base));

	data.at("invincible_time").get_to(character_base.m_invincible_time);
}

inline void to_json(nlohmann::json& data, const CharacterBase& character_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const PhysicalObjBase&>(character_base));

	nlohmann::json derived_json =
	{
		{ "invincible_time", character_base.m_invincible_time }
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
