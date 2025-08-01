#pragma once
#include "physical_obj_base.hpp"

#include "animator_base.hpp"
#include "../Part/modeler.hpp"

#include "gun_base.hpp"

class CharacterBase abstract : public PhysicalObjBase
{
public:
	CharacterBase(const std::string& name, const std::string& tag, const std::string& file_path, const MassKind mass_level_kind);
	virtual ~CharacterBase() = default;

	[[nodiscard]] std::shared_ptr<Modeler>		GetModeler()						{ return m_modeler; }
	[[nodiscard]] std::shared_ptr<AnimatorBase>	GetAnimator()				const	{ return m_animator; }
	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentAttachWeapon()	const	{ return m_current_equip_weapon; }
	[[nodiscard]] WeaponKind					GetCurrentEquipWeaponKind();

protected:
	#pragma region 武器
	/// @brief 武器を装備する
	template<obj_concepts::WeaponT WeaponObjT>
	void EquipWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		if (m_current_equip_weapon != weapon)
		{
			m_current_equip_weapon = weapon;
			m_current_equip_weapon->AttachOwner(m_modeler);
		}	
	}
	void EquipWeapon(const int obj_handle);
	/// @brief 武器の装備を解除する
	void UnequipWeapon();

	/// @brief 武器を装着する
	template<obj_concepts::WeaponT WeaponObjT>
	void AttachWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		m_attach_weapons[weapon->GetHolsterKind()] = weapon;
	}
	void AttachWeapon(const int obj_handle);
	/// @brief 武器の装着を解除する
	void DetachWeapon(const HolsterKind holster_kind);
	#pragma endregion


	#pragma region コライダー
	/// @brief キャラクターが標準的に持つコライダーを一括で作成する
	/// @brief カプセル(コライダー), 球(着地判定用トリガー), メッシュトリガー
	void CreateCharaBasisCollider(const float capsule_radius, const float sphere_radius);

	/// @brief カプセルの長さを計算
	void CalcCapsuleColliderLength();
	#pragma endregion


	/// @brief トランスフォーム情報を更新する
	void UpdateTransform(const VECTOR& look_dir, const float scale);

private:
	#pragma region コライダー
	/// @brief カプセルコライダーを作成
	void CreateCapsuleCollider(const float capsule_radius);

	/// @brief 着地トリガーを作成
	void CreateLandingTrigger(const float sphere_radius);
	#pragma endregion

protected:
	std::shared_ptr<Modeler>		m_modeler;
	std::shared_ptr<AnimatorBase>	m_animator;

	std::shared_ptr<WeaponBase>		m_current_equip_weapon;							// 現在装備している武器
	WeaponKind						m_current_equip_weapon_kind;					// 現在装備している武器の種類
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>> m_attach_weapons;	// 装着している武器

	std::shared_ptr<Capsule>		m_capsule_collider;
	float							m_capsule_length;
	float							m_capsule_radius;

	//float	m_hp;
	int								m_current_remaining_bullet_num;		// 残弾数
};
