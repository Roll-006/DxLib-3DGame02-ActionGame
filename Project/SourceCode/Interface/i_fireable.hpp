#pragma once
#include "../Part/ammo_holder.hpp"

class IFireable abstract
{
public:
	virtual ~IFireable() = default;

	[[nodiscard]] virtual std::shared_ptr<AmmoHolder> GetAmmoHolder() const abstract;
};
