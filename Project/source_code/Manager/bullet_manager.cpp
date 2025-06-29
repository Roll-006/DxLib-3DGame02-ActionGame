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
}

void BulletManager::Draw() const
{
	//int count = 0;
	for (const auto& bullet : m_bullets)
	{
		bullet->Draw();
	
		//DrawFormatString(0, 80 + 20 * count, 0xffffff, "%f, %f, %f", bullet->GetTransform()->GetPos(CoordinateKind::kWorld).x, bullet->GetTransform()->GetPos(CoordinateKind::kWorld).y, bullet->GetTransform()->GetPos(CoordinateKind::kWorld).z);
		//++count;
	}
}

void BulletManager::Shot(const VECTOR& pos, const VECTOR& dir, const float initial_velocity)
{
	const auto bullet = std::dynamic_pointer_cast<Bullet>(ObjectPoolManager::GetInstance()->GetObjectPool(ObjectPoolName.BULLET_POOL)->GetObj(ObjName.BULLET));
	if (bullet != nullptr)
	{
		bullet->OnShot(pos, dir, initial_velocity);
		AddBullet(bullet);
	}
}

void BulletManager::DeleteBullet(const int obj_handle)
{
	// éwíËÇÃíeä€ÇçÌèú
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
