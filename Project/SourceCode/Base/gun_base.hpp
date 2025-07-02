#pragma once
#include "weapon_base.hpp"
#include "../Data/Kind/gun_kind.hpp"

#include "../Object/camera.hpp"

#include "../Part/rifle_cartridge_object_pool.hpp"
#include "../Manager/rifle_cartridge_manager.hpp"

class GunBase abstract : public WeaponBase
{
public:
	GunBase(const std::string& name, const GunKind gun_kind, const std::string& file_path);
	virtual ~GunBase() = default;

	// 引き金を引く
	void PullTrigger() { m_is_pull_trigger = true; }
	/// @brief 引き金から手を離す
	void ReleaseTrigger() { m_is_pull_trigger = false; }

	/// @brief エイミング状態にする
	void ActivateAiming() { m_is_aiming = true; }
	/// @brief 非エイミング状態にする
	void DeactivateAiming() { m_is_aiming = false; }

	/// @brief レイキャスト用の線分を拡張した直線上にある点を設定する
	/// @brief 操作キャラの場合はカメラの座標
	/// @brief 非操作キャラの場合はターゲットの座標
	void SetPosOnRayLine(const VECTOR& point) { m_point_on_ray_line = point; }
	void SetAimDir(const VECTOR& aim_dir) { m_aim_dir = aim_dir; }

	[[nodiscard]] VECTOR  GetAimDir()			const { return m_aim_dir; }
	[[nodiscard]] VECTOR  GetMuzzlePos()		const;
	[[nodiscard]] VECTOR  GetEjectionPortPos()  const;
	[[nodiscard]] float   GetScopeScale()		const { return m_scope_scale; }
	[[nodiscard]] float   GetRange()			const { return m_range; }
	[[nodiscard]] float   GetInitialVelocity()	const { return m_initial_velocity; }
	[[nodiscard]] float	  GetDeceleration()		const { return m_deceleration; }
	[[nodiscard]] GunKind GetGunKind()			const { return m_gun_kind; }

	/// @brief 弾丸の発射位置を取得
	[[nodiscard]] VECTOR  GetFirstShotPos()		const;

	[[nodiscard]] bool    IsPullTrigger()       const { return m_is_pull_trigger; }
	[[nodiscard]] bool    IsAiming()			const { return m_is_aiming; }
	[[nodiscard]] bool    IsShot()		        const { return m_is_shot; }

protected:
	/// @brief 弾丸発射処理
	void Shot();

	/// @brief ターゲットの位置を計算
	virtual void CalcTargetPos() abstract;

protected:
	VECTOR  m_aim_dir;						// 狙う方向
	VECTOR  m_target_pos;					// 狙う位置
	VECTOR  m_muzzle_correct_pos;			// 銃口補正座標(銃口の座標を取得するためのオフセット)
	VECTOR  m_ejection_port_correct_pos;	// 薬莢を排出する開口部の座標を取得するためのオフセット
	VECTOR  m_point_on_ray_line;			// レイキャスト用の線分を拡張した直線上にある点

	float	m_scope_scale;					// スコープ倍率
	float	m_range;						// 射程
	float   m_initial_velocity;				// 弾丸の初速
	float   m_deceleration;					// 弾丸の減速度
	float   m_shot_interval_time;			// 弾丸が発射される時間間隔
	float	m_shot_timer;					// 弾丸を撃つためのタイマー
	bool    m_is_shot;						// 弾丸を撃つかを判定
	bool	m_is_pull_trigger;				// 銃の引き金が引かれているかを判定
	bool	m_is_aiming;					// 銃が構えられているかを判定

private:
	GunKind m_gun_kind;						// 銃の種類
};
