#pragma once
#include "physical_obj_base.hpp"
#include "../Interface/i_item.hpp"

#include "../Part/modeler.hpp"

#include "../Kind/weapon_kind.hpp"
#include "../Kind/holster_kind.hpp"

#include "../Accessor/obj_accessor.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"
#include "../Manager/effect_manager.hpp"

class WeaponBase abstract : public PhysicalObjBase, public IItem
{
public:
	WeaponBase(const std::string& name, const WeaponKind weapon_kind, const HolsterKind holster_kind);
	virtual ~WeaponBase() = default;

	void AddToObjManager()		override;
	void RemoveToObjManager()	override;

	void OnProjectPos() override;

	/// @brief 装備する持ち主をアタッチする
	void AttachOwner(const std::shared_ptr<Modeler>& owner_modeler, const std::string& owner_name);
	/// @brief 装備していた持ち主をデタッチする
	void DetachOwner();

	/// @brief 持ち主の手を追跡する
	virtual void TrackOwnerHand() abstract;
	/// @brief 持ち主のホルスター(武器収納位置)を追尾する
	virtual void TrackOwnerHolster()abstract;

	[[nodiscard]] const float						GetDeltaTime()		const override;
	[[nodiscard]] const ItemKind					GetItemKind()		const override	{ return ItemKind::kWeapon; }
	[[nodiscard]] const std::shared_ptr<const Modeler>	GetModeler()		const			{ return m_modeler; }
	[[nodiscard]] const float							GetPower()			const			{ return power; }
	[[nodiscard]] const std::string					GetOwnerName()		const			{ return m_owner_name; }
	[[nodiscard]] const WeaponKind					GetWeaponKind()		const			{ return weapon_kind; }
	[[nodiscard]] const HolsterKind					GetHolsterKind()	const			{ return holster_kind; }
	[[nodiscard]] const std::shared_ptr<Transform>	GetItemTransform()	const override	{ return m_item_effect_transform; }

protected:
	std::shared_ptr<Modeler>	m_modeler;
	std::shared_ptr<Modeler>	m_owner_modeler;	// 武器の持ち主であるオブジェクトのモデラー
	std::string					m_owner_name;
	std::shared_ptr<Transform>	m_item_effect_transform;
	
	float		power;
	VECTOR		basic_angle;
	float		basic_scale;
	VECTOR		hold_offset_pos;		// 手に持たれる際のオフセット座標
	VECTOR		hold_offset_angle;		// 手に持たれる際のオフセット角度
	VECTOR		hold_offset_scale;		// 手に持たれる際のオフセットスケール
	VECTOR		attach_offset_pos;		// ホルスターに装着されている際のオフセット座標
	VECTOR		attach_offset_angle;	// ホルスターに装着されている際のオフセット角度
	VECTOR		attach_offset_scale;	// ホルスターに装着されている際のオフセットスケール
	WeaponKind	weapon_kind;
	HolsterKind holster_kind;

	friend void from_json(const nlohmann::json& j_data, WeaponBase& weapon_base);
	friend void to_json	 (nlohmann::json& j_data, const WeaponBase& weapon_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, WeaponBase& weapon_base)
{
	from_json(j_data, static_cast<PhysicalObjBase&>(weapon_base));

	j_data.at("power")				.get_to(weapon_base.power);
	j_data.at("hold_offset_pos")		.get_to(weapon_base.hold_offset_pos);
	j_data.at("hold_offset_angle")	.get_to(weapon_base.hold_offset_angle);
	j_data.at("hold_offset_scale")	.get_to(weapon_base.hold_offset_scale);
	j_data.at("attach_offset_pos")	.get_to(weapon_base.attach_offset_pos);
	j_data.at("attach_offset_angle")	.get_to(weapon_base.attach_offset_angle);
	j_data.at("attach_offset_scale")	.get_to(weapon_base.attach_offset_scale);
	j_data.at("weapon_kind")			.get_to(weapon_base.weapon_kind);
	j_data.at("holster_kind")			.get_to(weapon_base.holster_kind);
}

inline void to_json(nlohmann::json& j_data, const WeaponBase& weapon_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const PhysicalObjBase&>(weapon_base));

	nlohmann::json derived_json =
	{
		{ "power",					weapon_base.power },
		{ "hold_offset_pos",		weapon_base.hold_offset_pos },
		{ "hold_offset_angle",		weapon_base.hold_offset_angle },
		{ "hold_offset_scale",		weapon_base.hold_offset_scale },
		{ "attach_offset_pos",		weapon_base.attach_offset_pos },
		{ "attach_offset_angle",	weapon_base.attach_offset_angle },
		{ "attach_offset_scale",	weapon_base.attach_offset_scale },
		{ "weapon_kind",			weapon_base.weapon_kind },
		{ "holster_kind",			weapon_base.holster_kind },
	};

	j_data = base_json;
	j_data.update(derived_json);
}
#pragma endregion
