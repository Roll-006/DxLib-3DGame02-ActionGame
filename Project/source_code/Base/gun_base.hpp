#pragma once
#include "weapon_base.hpp"
#include "../Data/Kind/gun_kind.hpp"

#include "../Object/camera.hpp"

class GunBase abstract : public WeaponBase
{
public:
	GunBase(const std::string& name, const GunKind gun_kind, const std::string& file_path) :
		WeaponBase					(name, file_path),
		m_aim_dir					(v3d::GetZeroV()),
		//m_muzzle_pos				(v3d::GetZeroV()),
		m_muzzle_correct_pos		(v3d::GetZeroV()),
		m_ejection_port_correct_pos	(v3d::GetZeroV()),
		m_point_on_ray_line			(v3d::GetZeroV()),
		m_scope_scale				(0.0f),
		m_range						(0.0f),
		m_is_aiming					(false),
		m_gun_kind					(gun_kind)
	{ }

	virtual ~GunBase() = default;

	/// @brief エイミング中にする
	void ActivateAiming()   { m_is_aiming = true; }
	/// @brief 非エイミング中にする
	void DeactivateAiming() { m_is_aiming = false; }

	/// @brief ショット状態にする
	void ActivateShot() { m_is_shot = true; }

	/// @brief レイキャスト用の線分を拡張した直線上にある点を設定する
	/// @brief プレイヤーの場合はカメラの座標
	/// @brief エネミーの場合はターゲットの座標
	void SetPointOnRayLine(const VECTOR& point) { m_point_on_ray_line = point; }
	void SetAimDir(const VECTOR& aim_dir) { m_aim_dir = aim_dir; }

	[[nodiscard]] VECTOR  GetAimDir()     const { return m_aim_dir; }
	[[nodiscard]] VECTOR  GetMuzzlePos()  const
	{
		const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
		const auto local_pos = m_transform->GetPos   (CoordinateKind::kLocal);

		return local_pos + VTransformSR(m_muzzle_correct_pos, world_m);
	}

	[[nodiscard]] float   GetScopeScale() const { return m_scope_scale; }
	[[nodiscard]] float   GetRange()	  const { return m_range; }
	[[nodiscard]] GunKind GetGunKind()	  const { return m_gun_kind; }

	[[nodiscard]] bool    IsAiming()      const { return m_is_aiming; }

protected:
	/// @brief レイキャスト用の光線の座標を計算
	void CalcRayPos()
	{
		if (!m_is_aiming) { return; }

		// 光線の始点を計算
		Segment s1 = Segment(m_point_on_ray_line, m_point_on_ray_line + m_aim_dir);
		Segment s2 = Segment(m_point_on_ray_line, GetMuzzlePos());
		VECTOR  v1 = s1.GetEndPos() - s1.GetBeginPos();
		VECTOR  v2 = s2.GetEndPos() - s2.GetBeginPos();
		VECTOR  h  = math::GetProjectionVector(v2, v1);
		VECTOR  ray_begin_pos = s1.GetBeginPos() + h;

		// 光線の座標を更新
		auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
		ray->SetBeginPos(ray_begin_pos, true);
		ray->SetEndPos  (ray->GetBeginPos() + m_aim_dir * m_range, true);
	}

protected:
	VECTOR  m_aim_dir;						// 狙う方向
	//VECTOR  m_muzzle_pos;					// 銃口座標
	VECTOR  m_muzzle_correct_pos;			// 銃口補正座標(銃口の座標を取得するためのオフセット)
	VECTOR  m_ejection_port_correct_pos;	// 薬莢を排出する開口部の座標を取得するためのオフセット
	VECTOR  m_point_on_ray_line;			// レイキャスト用の線分を拡張した直線上にある点
	float	m_scope_scale;					// スコープ倍率
	float	m_range;						// 射程
	float   m_move_speed_bullet;
	float   m_shot_interval;
	float   m_is_shot;
	bool	m_is_aiming;					// 銃が構えられているかを判定

private:
	GunKind m_gun_kind;
};
