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

	/// @brief ’eŠÛ‚Ì”­Ë
	/// @param pos ‰ŠúÀ•W
	/// @param dir ”­Ë•ûŒü
	/// @param initial_velocity ‰‘¬
	void Shot(const VECTOR& pos, const VECTOR& dir, const float initial_velocity);

	/// @brief g—pÏ‚İ’eŠÛ‚ğíœ
	void DeleteBullet(const int obj_handle);

private:
	BulletManager();
	~BulletManager() override;

	void AddBullet(const std::shared_ptr<Bullet> bullet);

private:
	std::vector<std::shared_ptr<Bullet>> m_bullets;

	friend SingletonBase<BulletManager>;
};
