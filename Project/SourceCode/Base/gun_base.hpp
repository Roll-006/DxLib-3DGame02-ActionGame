#pragma once
#include "weapon_base.hpp"
#include "../Data/Kind/gun_kind.hpp"

#include "../Object/main_camera.hpp"

#include "../Manager/rifle_cartridge_manager.hpp"

class GunBase abstract : public WeaponBase
{
public:
	GunBase(const std::string& name, const GunKind gun_kind, const HolsterKind holster_kind);
	virtual ~GunBase() = default;

	// 引き金が引かれる
	void PullTrigger()    { m_on_pull_trigger = true; }
	/// @brief 引き金から離される
	void ReleaseTrigger() { m_on_pull_trigger = false; }
	//
	///// @brief 構えられる
	//void EnterAiming() { m_on_aiming = true; }
	///// @brief 非エイミング状態にする
	//void ExitAiming() { m_on_aiming = false; }

	void Shot();

	/// @brief 弾丸のリロード
	/// @param have_bullets キャラクターが所持している弾丸数(装填した分が引かれて返ってくる)
	void OnReload(int& have_bullets);

	void CalcShotTimer();

	/// @brief レイキャスト用の線分を拡張した直線上にある点を設定する
	/// @brief 操作キャラの場合はカメラの座標
	/// @brief 非操作キャラの場合はターゲットの座標
	void SetPosOnRayLine(const VECTOR& point) { m_point_on_ray_line = point; }
	void SetAimDir(const VECTOR& aim_dir) { m_aim_dir = aim_dir; }

	[[nodiscard]] std::shared_ptr<ShapeBase> GetDiffusionShape() const { return m_diffusion_shape; }
	[[nodiscard]] VECTOR	GetAimDir()				const { return m_aim_dir; }
	[[nodiscard]] VECTOR	GetShotDir()			const;
	[[nodiscard]] VECTOR	GetMuzzlePos()			const;
	[[nodiscard]] VECTOR	GetEjectionPortPos()	const;
	[[nodiscard]] float		GetScopeScale()			const { return m_scope_scale; }
	[[nodiscard]] float		GetRange()				const { return m_range; }
	[[nodiscard]] float		GetInitialVelocity()	const { return m_initial_velocity; }
	[[nodiscard]] float		GetDeceleration()		const { return m_deceleration; }
	[[nodiscard]] GunKind	GetGunKind()			const { return m_gun_kind; }

	/// @brief 弾丸の発射位置を取得
	[[nodiscard]] VECTOR	GetFirstShotPos()		const;

	/// @brief 残弾数を取得
	[[nodiscard]] int		GetCurrentRemainingBulletNum()	const { return m_current_remaining_bullet_num; }
	/// @brief 最大の残弾数を取得
	[[nodiscard]] int		GetMaxRemainingBulletNum()		const { return m_max_remaining_bullet_num; }

	[[nodiscard]] bool IsShot() const;

protected:
	/// @brief 拡散範囲を計算
	virtual void CalcDiffusionRange() abstract;

	/// @brief 射撃するターゲット座標を計算
	virtual void CalcTargetPos() abstract;

protected:
	static constexpr float kDiffusionDistance = 1500.0f;		// 拡散範囲が位置する座標までの距離

	std::shared_ptr<ShapeBase> m_diffusion_shape;				// 拡散範囲指定用の図形

	VECTOR  m_aim_dir;						// 狙う方向
	VECTOR  m_target_pos;					// 狙う位置
	VECTOR  m_muzzle_offset_pos;			// 銃口の座標を取得するためのオフセット
	VECTOR  m_ejection_port_offset_pos;		// 薬莢を排出する開口部の座標を取得するためのオフセット
	VECTOR  m_point_on_ray_line;			// レイキャスト用の線分を拡張した直線上にある点

	int		m_current_remaining_bullet_num;	// 現在の残弾数
	int     m_max_remaining_bullet_num;		// 最大残弾数

	float	m_scope_scale;					// スコープ倍率
	float	m_range;						// 射程
	float   m_initial_velocity;				// 弾丸の初速
	float   m_deceleration;					// 弾丸の減速度
	float   m_shot_interval_time;			// 弾丸が発射される時間間隔
	float	m_shot_timer;					// 弾丸を撃つためのタイマー
	bool	m_on_pull_trigger;				// 銃の引き金が引かれているかを判定

private:
	GunKind m_gun_kind;						// 銃の種類
};
