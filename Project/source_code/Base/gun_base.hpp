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
		m_muzzle_correct_pos		(v3d::GetZeroV()),
		m_ejection_port_correct_pos	(v3d::GetZeroV()),
		m_point_on_ray_line			(v3d::GetZeroV()),
		m_scope_scale				(0.0f),
		m_range						(0.0f),
		m_move_speed_bullet			(0.0f),
		m_shot_interval_time		(0.0f),
		m_shot_timer				(0.0f),
		m_is_shot					(false),
		m_is_aiming					(false),
		m_gun_kind					(gun_kind)
	{ }

	virtual ~GunBase() = default;

	/// @brief エイミング中にする
	void ActivateAiming()   { m_is_aiming = true; }
	/// @brief 非エイミング中にする
	void DeactivateAiming() { m_is_aiming = false; }

	/// @brief ショット状態にする
	//void ActivateShot() { m_is_shot = true; }

	/// @brief レイキャスト用の線分を拡張した直線上にある点を設定する
	/// @brief プレイヤーの場合はカメラの座標
	/// @brief エネミーの場合はターゲットの座標
	void SetPointOnRayLine(const VECTOR& point) { m_point_on_ray_line = point; }
	void SetAimDir(const VECTOR& aim_dir) { m_aim_dir = aim_dir; }

	[[nodiscard]] VECTOR  GetAimDir()			const { return m_aim_dir; }
	[[nodiscard]] VECTOR  GetMuzzlePos()		const;
	[[nodiscard]] VECTOR  GetEjectionPortPos()  const;
	[[nodiscard]] float   GetScopeScale()		const { return m_scope_scale; }
	[[nodiscard]] float   GetRange()			const { return m_range; }
	[[nodiscard]] GunKind GetGunKind()			const { return m_gun_kind; }

	[[nodiscard]] bool    IsAiming()			const { return m_is_aiming; }
	[[nodiscard]] bool	  IsShot()				const { return m_is_shot; }

protected:
	/// @brief レイキャスト用の光線の座標を計算
	void CalcRayPos();

	/// @brief 弾丸を発射するかを判定する
	void JudgeShot();

protected:
	VECTOR  m_aim_dir;						// 狙う方向
	VECTOR  m_muzzle_correct_pos;			// 銃口補正座標(銃口の座標を取得するためのオフセット)
	VECTOR  m_ejection_port_correct_pos;	// 薬莢を排出する開口部の座標を取得するためのオフセット
	VECTOR  m_point_on_ray_line;			// レイキャスト用の線分を拡張した直線上にある点
	float	m_scope_scale;					// スコープ倍率
	float	m_range;						// 射程
	float   m_move_speed_bullet;			// 弾丸の移動速度
	float   m_shot_interval_time;			// 弾丸が発射される時間間隔
	float	m_shot_timer;					// 弾丸を撃つためのタイマー
	bool    m_is_shot;						// 弾丸を撃つかを判定
	bool	m_is_aiming;					// 銃が構えられているかを判定

private:
	GunKind m_gun_kind;						// 銃の種類
};
