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
	/// @brief •Ší‚ğ‘•”õ‚³‚¹‚é
	virtual void EquipWeapon(const std::shared_ptr<WeaponBase>& weapon , const WeaponSlotKind slot_kind) abstract;
	/// @brief •Ší‚Ì‘•”õ‚ğ‰ğœ
	virtual void UnequipWeapon(const WeaponSlotKind slot_kind)				abstract;

	/// @brief •Ší‚ğè‚É‚½‚¹‚é
	virtual void HoldWeapon(const std::shared_ptr<WeaponBase>& weapon)		abstract;
	virtual void HoldWeapon(const int obj_handle)							abstract;
	/// @brief è‚É‚Á‚Ä‚¢‚é•Ší‚ğ•ú‚·
	virtual void ReleaseWeapon()											abstract;

	/// @brief •Ší‚ğ‘•’…‚·‚é
	virtual void AttachWeapon(const std::shared_ptr<WeaponBase>& weapon)	abstract;
	virtual void AttachWeapon(const int obj_handle)							abstract;
	/// @brief •Ší‚Ì’…’E‚·‚é
	virtual void DetachWeapon(const std::shared_ptr<WeaponBase>& weapon)	abstract;
	virtual void DetachWeapon(const HolsterKind holster_kind)				abstract;


	#pragma region Getter
	virtual [[nodiscard]] const std::shared_ptr<WeaponBase> GetCurrentEquipWeapon(const WeaponSlotKind slot_kind) const abstract;
	virtual [[nodiscard]] const std::shared_ptr<WeaponBase> GetCurrentHeldWeapon() abstract;
	virtual [[nodiscard]] const std::shared_ptr<WeaponBase> GetCurrentAttachWeapon(const HolsterKind holster_kind) const abstract;
	virtual [[nodiscard]] const WeaponKind GetCurrentEquipWeaponKind(const WeaponSlotKind slot_kind) abstract;
	virtual [[nodiscard]] const WeaponKind GetCurrentHeldWeaponKind() abstract;
	virtual [[nodiscard]] const WeaponKind GetCurrentAttachWeaponKind(const HolsterKind holster_kind) const abstract;
	#pragma endregion
};
