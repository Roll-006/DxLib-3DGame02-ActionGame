#pragma once
#include "weapon_base.hpp"
#include "../Data/Kind/gun_kind.hpp"

#include "../Object/camera.hpp"

class GunBase abstract : public WeaponBase
{
public:
	GunBase(const std::string& name, const GunKind gun_kind, const std::string& file_path) :
		WeaponBase			(name, file_path),
		m_aim_dir			(v3d::GetZeroV()),
		m_muzzle_pos		(v3d::GetZeroV()),
		m_muzzle_correct_pos(v3d::GetZeroV()),
		m_scope_scale		(0.0f),
		m_range				(0.0f),
		m_is_aiming			(false),
		m_gun_kind			(gun_kind)
	{ }

	virtual ~GunBase() = default;

	void SetAimDir(const VECTOR& aim_dir) { m_aim_dir = aim_dir; }

	[[nodiscard]] VECTOR  GetAimDir()     const { return m_aim_dir; }
	[[nodiscard]] VECTOR  GetMuzzlePos()  const { return m_muzzle_pos; }
	[[nodiscard]] float   GetScopeScale() const { return m_scope_scale; }
	[[nodiscard]] float   GetRange()	  const { return m_range; }
	[[nodiscard]] GunKind GetGunKind()	  const { return m_gun_kind; }

	[[nodiscard]] bool    IsAiming()      const { return m_is_aiming; }

protected:
	/// @brief 銃口の座標を計算
	void CalcMuzzlePos()
	{
		const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
		const auto local_pos = m_transform->GetPos   (CoordinateKind::kLocal);

		m_muzzle_pos = local_pos + VTransformSR(m_muzzle_correct_pos, world_m);
	}

protected:
	VECTOR  m_aim_dir;				// 狙う方向
	VECTOR  m_muzzle_pos;			// 銃口座標
	VECTOR  m_muzzle_correct_pos;	// 銃口補正座標(銃口の座標を取得するためのオフセット)
	float	m_scope_scale;			// スコープ倍率
	float	m_range;				// 射程
	bool	m_is_aiming;			// 銃がのぞき込まれている(構えられている)

private:
	GunKind m_gun_kind;
};
