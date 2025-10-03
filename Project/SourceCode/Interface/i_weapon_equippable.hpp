#pragma once
#include "../Base/weapon_base.hpp"

enum class WeaponSlotKind
{
	kMain,
	kSub,
};

class IWeaponEquippable abstract
{
public:
	/// @brief 武器を装備させる(プレイヤーはナイフ系統のみ例外的に処理する)
	virtual void EquipWeapon(const std::shared_ptr<WeaponBase>& weapon , const WeaponSlotKind slot_kind) abstract;
	/// @brief 武器の装備を解除
	virtual void UnequipWeapon(const WeaponSlotKind slot_kind)				abstract;

	/// @brief 武器を手に持たせる
	virtual void HoldWeapon(const std::shared_ptr<WeaponBase>& weapon)		abstract;
	virtual void HoldWeapon(const int obj_handle)							abstract;
	/// @brief 手に持っている武器を放す
	virtual void ReleaseWeapon()											abstract;

	/// @brief 武器を装着する
	virtual void AttachWeapon(const std::shared_ptr<WeaponBase>& weapon)	abstract;
	virtual void AttachWeapon(const int obj_handle)							abstract;
	/// @brief 武器の着脱する
	virtual void DetachWeapon(const std::shared_ptr<WeaponBase>& weapon)	abstract;
	virtual void DetachWeapon(const HolsterKind holster_kind)				abstract;


	#pragma region Getter
	virtual [[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentEquipWeapon		(const WeaponSlotKind slot_kind) const abstract;
	virtual [[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentHeldWeapon		() abstract;
	virtual [[nodiscard]] std::shared_ptr<WeaponBase>	GetCurrentAttachWeapon		(const HolsterKind holster_kind) const abstract;
	virtual [[nodiscard]] WeaponKind					GetCurrentEquipWeaponKind	(const WeaponSlotKind slot_kind) abstract;
	virtual [[nodiscard]] WeaponKind					GetCurrentHeldWeaponKind	() abstract;
	virtual [[nodiscard]] WeaponKind					GetCurrentAttachWeaponKind	(const HolsterKind holster_kind) const abstract;
	#pragma endregion
};
