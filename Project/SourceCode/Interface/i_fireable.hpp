#pragma once
#include "../Part/ammo_holder.hpp"

class IFireable abstract
{
public:
	virtual ~IFireable() = default;

	/// @brief c’e”‚ğİ’è‚·‚é
	/// @param remaining_bullet_num 
	virtual void SetRemainingBulletNum(const int remaining_bullet_num) abstract;

	[[nodiscard]] virtual int GetCurrentRemainingBulletNum() const abstract;

	[[nodiscard]] virtual std::shared_ptr<AmmoHolder> GetAmmoHolder() const abstract;
};
