#pragma once
#include "physical_obj_base.hpp"

#include "../Part/modeler.hpp"
#include "animator_base.hpp"

#include "gun_base.hpp"

class CharacterBase abstract : public PhysicalObjBase
{
public:
	CharacterBase(const std::string& name, const std::string& tag, const MassKind mass_level_kind);
	virtual ~CharacterBase() = default;

	void AddToObjManager() override;

	/// @brief 残弾数を設定する
	/// @param remaining_bullet_num 
	void SetRemainingBulletNum(const int remaining_bullet_num) { m_current_remaining_bullet_num = remaining_bullet_num; }


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<Modeler>		GetModeler()					const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<AnimatorBase>	GetAnimator()					const { return m_animator; }
	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentHeldWeapon()			const { return m_current_held_weapon; }
	[[nodiscard]] WeaponKind					GetCurrentHeldWeaponKind();
	[[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentAttachWeapon		(const HolsterKind holster_kind) const;
	[[nodiscard]] WeaponKind					GetCurrentAttachWeaponKind	(const HolsterKind holster_kind) const;
	[[nodiscard]] int							GetCurrentRemainingBulletNum()	const { return m_current_remaining_bullet_num; }
	#pragma endregion


	#pragma region 武器
	/// @brief 武器を手に持たせる
	template<obj_concepts::WeaponT WeaponObjT>
	void HoldWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		m_current_held_weapon = weapon;
		m_current_held_weapon->AttachOwner(m_modeler);
	}
	void HoldWeapon(const int obj_handle);

	/// @brief 手に持っている武器を放す
	void ReleaseWeapon();

	/// @brief 武器を装着する
	template<obj_concepts::WeaponT WeaponObjT>
	void AttachWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		m_attach_weapons[weapon->GetHolsterKind()] = weapon;
		m_attach_weapons[weapon->GetHolsterKind()]->AttachOwner(m_modeler);
	}
	void AttachWeapon(const int obj_handle);

	/// @brief 武器の着脱する
	template<obj_concepts::WeaponT WeaponObjT>
	void DetachWeapon(const std::shared_ptr<WeaponObjT> weapon)
	{
		// 自身が装着されていれば着脱する
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
	#pragma region コライダー
	/// @brief キャラクターが標準的に持つコライダーを一括で作成する
	/// @brief カプセル(コライダー), 球(着地判定用トリガー), メッシュトリガー
	void CreateCharaBasisCollider(const float capsule_radius, const float sphere_radius);

	/// @brief カプセルの長さを計算
	void CalcCapsuleColliderLength();
	#pragma endregion


	/// @brief 見ている方向を回転に適用する
	void ApplyLookDirToRot(const VECTOR& look_dir);

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

	std::shared_ptr<WeaponBase>		m_current_held_weapon;							// 現在手に持っている武器
	std::unordered_map<HolsterKind, std::shared_ptr<WeaponBase>> m_attach_weapons;	// 装着している武器

	std::shared_ptr<Capsule>		m_capsule_collider;
	float							m_capsule_length;
	float							m_capsule_radius;

	//float	m_hp;
	int								m_current_remaining_bullet_num;		// 残弾数
};
