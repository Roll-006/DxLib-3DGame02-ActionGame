#include "bullet_object_pool.hpp"

BulletObjectPool::BulletObjectPool() : 
	ObjectPoolBase(ObjectPoolName.BULLET_POOL)
{
	m_pool_size[ObjName.BULLET] = kBulletPoolSize;

	for (int i = 0; i < m_pool_size.at(ObjName.BULLET); ++i)
	{
		const auto bullet = std::make_shared<Bullet>();

		CreateObj(bullet);

		ObjManager		::GetInstance()->AddObj		   (bullet);
		CollisionManager::GetInstance()->AddCollideObj (bullet);
		PhysicsManager	::GetInstance()->AddPhysicalObj(bullet);
	}
}

BulletObjectPool::~BulletObjectPool()
{

}
