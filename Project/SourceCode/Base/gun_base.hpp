#pragma once
#include "weapon_base.hpp"
#include "../Kind/gun_kind.hpp"

#include "../Object/main_camera.hpp"
#include "../Data/recoil_data.hpp"

#include "../Manager/rifle_cartridge_manager.hpp"
#include "../Interface/i_loadable_ammo.hpp"

class GunBase abstract : public WeaponBase
{
public:
	GunBase(const std::string& name, const GunKind gun_kind, const HolsterKind holster_kind);
	virtual ~GunBase() = default;

	/// @brief 持ち主の手を追跡する
	void TrackOwnerHand() override;
	/// @brief 持ち主のホルスター(武器収納位置)を追尾する
	void TrackOwnerHolster()override;

	/// @brief 引き金が引かれる
	void PullTrigger()    { m_on_pull_trigger = true; }
	/// @brief 引き金から離される
	void ReleaseTrigger() { m_on_pull_trigger = false; }

	void OnShot();

	/// @brief 弾丸のリロード
	/// @param have_bullets キャラクターが所持している弾丸数
	/// @return 装填した分が引かれた残弾数
	[[nodiscard]] int OnReload(const int have_bullets);

	virtual void InitCrossHairRange() abstract;

	/// @brief 拡散範囲を計算
	virtual void CalcCrossHairRange(const VECTOR& owner_move_velocity) abstract;
	virtual void CalcCrossHairRangeShot()	abstract;
	virtual void CalcCrossHairPos()			abstract;
	virtual void CalcTransforms()			abstract;

	/// @brief 射撃するターゲット座標を計算
	virtual void CalcTargetPos() abstract;

	void CalcShotTimer();

	/// @brief レイキャスト用の線分を拡張した直線上にある点を設定する
	/// @brief 操作キャラの場合はカメラの座標
	/// @brief 非操作キャラの場合はターゲットの座標
	void SetPosOnRay(const VECTOR& point) { m_point_on_ray = point; }
	void SetAimDir(const VECTOR& aim_dir) { m_aim_dir = aim_dir; }


	#pragma region Getter
	[[nodiscard]] std::shared_ptr<ILoadableAmmo>	GetMagazine()		const { return m_magazine; }
	[[nodiscard]] std::shared_ptr<ShapeBase> GetDiffusionShape()		const { return m_cross_hair_shape; }
	[[nodiscard]] std::shared_ptr<Transform> GetMuzzleTransform()		const { return m_muzzle_transform; }
	[[nodiscard]] std::shared_ptr<Transform> GetEjectionPortTransform()	const { return m_ejection_port_transform; }
	[[nodiscard]] std::shared_ptr<Transform> GetLoadTransform()			const { return m_load_transform; }
	[[nodiscard]] VECTOR	 GetAimDir()					const { return m_aim_dir; }
	[[nodiscard]] VECTOR	 GetShotDir()					const;
	[[nodiscard]] float		 GetScopeScale()				const { return scope_scale; }
	[[nodiscard]] float		 GetRange()						const { return range; }
	[[nodiscard]] float		 GetInitialVelocity()			const { return initial_velocity; }
	[[nodiscard]] float		 GetDeceleration()				const { return deceleration; }
	[[nodiscard]] RecoilData GetRecoilData()				const { return recoil_data; }
	[[nodiscard]] GunKind	 GetGunKind()					const { return gun_kind; }

	/// @brief 弾丸の発射位置を取得
	[[nodiscard]] VECTOR	 GetFirstShotPos()				const;

	/// @brief 残弾数を取得
	[[nodiscard]] int		 GetCurrentRemainingBulletNum()	const { return m_current_remaining_bullet_num; }
	/// @brief 最大の残弾数を取得
	[[nodiscard]] int		 GetMaxRemainingBulletNum()		const { return max_remaining_bullet_num; }

	[[nodiscard]] bool		 IsShot() const;
	[[nodiscard]] bool		 CanReload() const;
	#pragma endregion

protected:
	void CalcTransform(std::shared_ptr<Transform>& transform, const VECTOR& offset);

protected:
	GunKind		gun_kind;						// 銃の種類
	float		cross_hair_distance;			// 拡散範囲が位置する座標までの距離
	int			max_remaining_bullet_num;		// 最大残弾数
	float		range;							// 射程
	float		initial_velocity;				// 弾丸の初速
	float		deceleration;					// 弾丸の減速度
	RecoilData	recoil_data;

	float		shot_interval_time;				// 弾丸が発射される時間間隔


	std::shared_ptr<ILoadableAmmo>		m_magazine;

	std::shared_ptr<ShapeBase>			m_cross_hair_shape;		// 拡散範囲指定用の図形
	std::shared_ptr<Transform>			m_muzzle_transform;
	std::shared_ptr<Transform>			m_ejection_port_transform;
	std::shared_ptr<Transform>			m_load_transform;

	VECTOR		m_aim_dir;						// 狙う方向
	VECTOR		m_target_pos;					// 狙う位置
	VECTOR		m_point_on_ray;					// レイキャスト用の線分を拡張した直線上にある点

	int			m_current_remaining_bullet_num;	// 現在の残弾数

	float		scope_scale;					// スコープ倍率
	float		m_shot_timer;					// 弾丸を撃つためのタイマー
	bool		m_on_pull_trigger;				// 銃の引き金が引かれているかを判定

	friend void from_json(const nlohmann::json& j_data, GunBase& gun_base);
	friend void to_json(nlohmann::json& j_data, const GunBase& gun_base);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, GunBase& gun_base)
{
	from_json(j_data, static_cast<WeaponBase&>(gun_base));

	j_data.at("cross_hair_distance")		.get_to(gun_base.cross_hair_distance);
	j_data.at("max_remaining_bullet_num")	.get_to(gun_base.max_remaining_bullet_num);
	j_data.at("range")					.get_to(gun_base.range);
	j_data.at("initial_velocity")			.get_to(gun_base.initial_velocity);
	j_data.at("deceleration")				.get_to(gun_base.deceleration);
	j_data.at("recoil_data")				.get_to(gun_base.recoil_data);
	j_data.at("shot_interval_time")		.get_to(gun_base.shot_interval_time);
	j_data.at("gun_kind")					.get_to(gun_base.gun_kind);
}

inline void to_json(nlohmann::json& j_data, const GunBase& gun_base)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const WeaponBase&>(gun_base));

	nlohmann::json derived_json =
	{
		{ "cross_hair_distance",		gun_base.cross_hair_distance },
		{ "max_remaining_bullet_num",	gun_base.max_remaining_bullet_num },
		{ "range",						gun_base.range },
		{ "initial_velocity",			gun_base.initial_velocity },
		{ "deceleration",				gun_base.deceleration },
		{ "recoil_data",				gun_base.recoil_data },
		{ "shot_interval_time",			gun_base.shot_interval_time },
		{ "gun_kind",					gun_base.gun_kind },
	};

	j_data = base_json;
	j_data.update(derived_json);
}
#pragma endregion
