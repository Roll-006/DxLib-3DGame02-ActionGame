#pragma once
#include "../Base/singleton_base.hpp"
#include "../Manager/object_pool_manager.hpp"
#include "../Object/Bullet.hpp"

class BulletManager final : public SingletonBase<BulletManager>
{
public:
	void Update();
	void LateUpdate();
	void Draw() const;

	/// @brief 弾丸の発射
	/// @param pos 初期座標
	/// @param dir 発射方向
	/// @param initial_velocity 初速
	/// @param range 射程
	void Shot(const VECTOR& pos, const VECTOR& dir, const float initial_velocity, const float range);

	/// @brief 使用済み弾丸を削除
	void DeleteBullet(const int obj_handle);

	void AddHitPos(const VECTOR& hit_pos)
	{
		m_hit_pos.emplace_back(hit_pos);
	}

private:
	BulletManager();
	~BulletManager() override;

	void AddBullet(const std::shared_ptr<Bullet> bullet);

private:
	std::vector<std::shared_ptr<Bullet>> m_bullets;
	std::vector<VECTOR> m_hit_pos;	// MEMO : 仮の衝突座標

	friend SingletonBase<BulletManager>;
};
