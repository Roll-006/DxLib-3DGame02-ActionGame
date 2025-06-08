#pragma once
#include "weapon_base.hpp"
#include "../Data/Kind/gun_kind.hpp"

class GunBase abstract : public WeaponBase
{
public:
	GunBase(const std::string& name, const GunKind gun_kind, const std::string& file_path) :
		WeaponBase		(name, file_path),
		m_gun_kind		(gun_kind),
		m_scope_scale	(0.0f)
	{ }

	virtual ~GunBase() = default;

	[[nodiscard]] GunKind GetGunKind()	  const { return m_gun_kind; }
	[[nodiscard]] float   GetScopeScale() const { return m_scope_scale; }

protected:
	void SetScopeScale(const float scale) { m_scope_scale = scale; }

private:
	GunKind m_gun_kind;
	float	m_scope_scale;
};
