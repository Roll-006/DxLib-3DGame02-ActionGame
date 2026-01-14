#pragma once
#include "physical_obj_base.hpp"

#include "../Part/character_collider_creator.hpp"
#include "../Part/gauge.hpp"
#include "../Part/modeler.hpp"
#include "../Animator/animator.hpp"

#include "gun_base.hpp"
#include "../Kind/health_part_kind.hpp"

class CharacterBase abstract : public PhysicalObjBase
{
public:
	CharacterBase(const std::string& name, const std::string& tag);
	virtual ~CharacterBase() = default;

	void AddToObjManager()		override;
	void RemoveToObjManager()	override;

	void UpdateLocomotion();

	void InitMoveOffset();
	void CalcCorrectMoveDir();
	void AllowCalcLookDir() { m_is_calc_look_dir = true; }

	/// @brief 強制的に無敵状態にする
	void ActivateInvincibleForcibly()	{ m_is_invincible = true; }
	/// @brief 強制的に無敵状態を解除する
	void DeactivateInvincibleForcibly() { m_is_invincible = false; }

	virtual void OnDamage(const HealthPartKind part_kind, const float damage) abstract;

	#pragma region Getter
	[[nodiscard]] const bool								IsInvincible()		const				{ return m_is_invincible; }
	[[nodiscard]] const bool								IsAlive()			const				{ return m_health.at(HealthPartKind::kMain)->IsAlive(); }
	[[nodiscard]] const bool								IsGoUpHill(const Triangle& hit_triangle) const;
	[[nodiscard]] const std::shared_ptr<Modeler>			GetModeler()		const				{ return m_modeler; }
	[[nodiscard]] const std::shared_ptr<const Animator>		GetAnimator()		const				{ return m_animator; }
	[[nodiscard]] const VECTOR								GetMoveDir(const TimeKind kind)	const	{ return m_move_dir.at(kind); }
	[[nodiscard]] const VECTOR								GetLookDir(const TimeKind kind)	const	{ return m_look_dir.at(kind); }
	[[nodiscard]] const VECTOR								GetMoveVelocity()	const				{ return m_move_velocity; }
	[[nodiscard]] const std::shared_ptr<Gauge>				GetHealth(const HealthPartKind kind)	{ return m_health.at(kind); }
	#pragma endregion

protected:
	void JudgeInvincible();

	/// @brief 見ている方向を回転に適用する
	void ApplyLookDirToRot();

private:
	void CalcMoveDir();
	void CalcLookDir();
	void CalcMoveVelocity();

protected:
	float invincible_time;

	std::shared_ptr<Modeler>					m_modeler;
	std::shared_ptr<Animator>					m_animator;
	std::shared_ptr<CharacterColliderCreator>	m_collider_creator;

	std::unordered_map<TimeKind, VECTOR>		m_move_dir;					// 移動方向(WARNING : 長さは0～1の範囲を取る)
	std::unordered_map<TimeKind, VECTOR>		m_look_dir;					// 向いている方向
	VECTOR										m_move_offset;				// 移動velocityを補正
	VECTOR										m_destination_pos;			// 補正先座標
	float										m_move_speed;
	float										m_move_dir_offset_speed;	// 移動方向を補正する速度
	float										m_look_dir_offset_speed;	// 見る方向の補正速度
	bool										m_is_calc_look_dir;

	std::shared_ptr<WeaponBase>											m_current_held_weapon;	// 現在手に持っている武器
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>>		m_attach_weapons;		// 装着している武器

	std::unordered_map<HealthPartKind, std::shared_ptr<Gauge>>	m_health;

	float m_invincible_timer;
	bool  m_is_invincible;

	friend void from_json(const nlohmann::json& j_data, CharacterBase& character_base);
	friend void to_json  (nlohmann::json& j_data, const CharacterBase& character_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, CharacterBase& character_base)
{
	from_json(j_data, static_cast<PhysicalObjBase&>(character_base));

	j_data.at("invincible_time").get_to(character_base.invincible_time);
}

inline void to_json(nlohmann::json& j_data, const CharacterBase& character_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const PhysicalObjBase&>(character_base));

	nlohmann::json derived_json =
	{
		{ "invincible_time", character_base.invincible_time }
	};

	j_data = base_json;
	j_data.update(derived_json);
}
#pragma endregion
