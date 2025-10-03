#pragma once
#include "physical_obj_base.hpp"
#include "../Interface/i_item.hpp"

#include "../Part/modeler.hpp"

#include "../Kind/weapon_kind.hpp"
#include "../Kind/holster_kind.hpp"

#include "../Manager/obj_manager.hpp"
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

	/// @brief 装備する持ち主をアタッチする
	void AttachOwner(const std::shared_ptr<Modeler>& owner_modeler, const std::string& owner_name);
	/// @brief 装備していた持ち主をデタッチする
	void DetachOwner();

	/// @brief 持ち主の手を追跡する
	void TrackOwnerHand();
	/// @brief 持ち主のホルスター(武器収納位置)を追尾する
	void TrackOwnerHolster();

	[[nodiscard]] float						GetDeltaTime()	 const override;
	[[nodiscard]] std::shared_ptr<Modeler>	GetModeler()	 const			{ return m_modeler; }
	[[nodiscard]] float						GetPower()		 const			{ return m_power; }
	[[nodiscard]] std::string				GetOwnerName()	 const			{ return m_owner_name; }
	[[nodiscard]] ItemKind					GetItemKind()	 const override	{ return m_item_kind; }
	[[nodiscard]] WeaponKind				GetWeaponKind()	 const			{ return m_weapon_kind; }
	[[nodiscard]] HolsterKind				GetHolsterKind() const			{ return m_holster_kind; }

protected:
	/// @brief 補正値を設定
	/// @param hold_pos 手に持たれる際のオフセット座標
	/// @param hold_angle 手に持たれる際のオフセット角度
	/// @param attach_pos ホルスターに装着されている際のオフセット座標
	/// @param attach_angle ホルスターに装着されている際のオフセット角度
	/// @param scale オフセットスケール
	void SetOffset(
		const VECTOR& hold_pos,
		const VECTOR& hold_angle,
		const VECTOR& hold_scale,
		const VECTOR& attach_pos,
		const VECTOR& attach_angle,
		const VECTOR& attach_scale);

	void SetOffset(
		const VECTOR& hold_pos,
		const VECTOR& hold_angle,
		const float   hold_scale,
		const VECTOR& attach_pos,
		const VECTOR& attach_angle,
		const float   attach_scale);

protected:
	std::shared_ptr<Modeler> m_modeler;
	std::shared_ptr<Modeler> m_owner_modeler;	// 武器の持ち主であるオブジェクトのモデラー
	std::string				 m_owner_name;
	float					 m_power;
	
private:
	VECTOR m_hold_offset_pos;		// 手に持たれる際のオフセット座標
	VECTOR m_hold_offset_angle;		// 手に持たれる際のオフセット角度
	VECTOR m_hold_offset_scale;		// 手に持たれる際のオフセットスケール
	VECTOR m_attach_offset_pos;		// ホルスターに装着されている際のオフセット座標
	VECTOR m_attach_offset_angle;	// ホルスターに装着されている際のオフセット角度
	VECTOR m_attach_offset_scale;	// ホルスターに装着されている際のオフセットスケール

	ItemKind	m_item_kind;
	WeaponKind	m_weapon_kind;
	HolsterKind m_holster_kind;
};
