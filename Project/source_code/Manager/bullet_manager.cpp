#include "bullet_manager.hpp"

BulletManager::BulletManager()
{

}

BulletManager::~BulletManager()
{

}

void BulletManager::Update()
{
	for (const auto& bullet : m_bullets)
	{
		bullet->Update();
	}
}

void BulletManager::LateUpdate()
{
	for (const auto& bullet : m_bullets)
	{
		bullet->LateUpdate();
	}
	
	// 生存していない弾丸は削除
	const auto remove_bullet = std::find_if(m_bullets.begin(), m_bullets.end(), [](const std::shared_ptr<Bullet> bullet)
	{
		return !bullet->IsAlive();
	});
	if (remove_bullet != m_bullets.end())
	{
		ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->ReturnObj(*remove_bullet);
		m_bullets.erase(remove_bullet);
	}
}

void BulletManager::Draw() const
{
	DrawFormatString(0,  80, 0xffffff, "pool_size    : %d", ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->GetPoolSize(ObjName.BULLET));
	DrawFormatString(0, 100, 0xffffff, "manager_size : %d", m_bullets.size());

	int count = 0;
	for (const auto& bullet : m_bullets)
	{
		bullet->Draw();
	
		const auto pos = bullet->GetTransform()->GetPos(CoordinateKind::kWorld);
		DrawFormatString(  0, 120 + 20 * count, 0xffffff, "%f, %f, %f", pos.x, pos.y, pos.z);
		DrawFormatString(400, 120 + 20 * count, 0xffffff, "%d", bullet->IsAlive());
		++count;
	}

	for (const auto& pos : m_hit_pos)
	{
		DrawSphere3D(pos, 5, 16, 0xff0000, 0xff0000, TRUE);
	}
}

void BulletManager::Shot(const VECTOR& pos, const VECTOR& dir, const float initial_velocity, const float range)
{
	// プールから弾丸を取得し、有効であれば発射
	const auto bullet = std::dynamic_pointer_cast<Bullet>(ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->GetObj(ObjName.BULLET));
	if (bullet != nullptr)
	{
		bullet->OnShot(pos, dir, initial_velocity, range);
		AddBullet(bullet);
	}
}

void BulletManager::DeleteBullet(const int obj_handle)
{
	// 指定の弾丸を削除
	const auto remove_bullet = std::find_if(m_bullets.begin(), m_bullets.end(), [=](const std::shared_ptr<Bullet> bullet)
	{
		return bullet->GetObjHandle() == obj_handle;
	});

	if(remove_bullet != m_bullets.end())
	{
		ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->ReturnObj(*remove_bullet);
		m_bullets.erase(remove_bullet);
	}
}

void BulletManager::AddBullet(const std::shared_ptr<Bullet> bullet)
{
	if (std::find(m_bullets.begin(), m_bullets.end(), bullet) == m_bullets.end())
	{
		m_bullets.emplace_back(bullet);
	}
}
