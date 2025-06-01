#pragma once
#include "weapon_base.hpp"
#include "../Data/Kind/gun_kind.hpp"

class GunBase : public WeaponBase
{
public:
	GunBase(const std::string& name, const GunKind gun_kind, const std::string& file_path) :
		WeaponBase(name, file_path)
	{ }

	virtual ~GunBase() = default;

	[[nodiscard]] GunKind GetGunKind()const { return m_gun_kind; }

private:
	GunKind m_gun_kind;
};
