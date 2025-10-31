#include "enemy_object_pool.hpp"

EnemyObjectPool::EnemyObjectPool() :
	ObjectPoolBase(ObjectPoolName.ENEMY_POOL)
{
	m_pool_size[ObjName.ZOMBIE] = kPoolSize;

	// ゾンビ
	for (size_t i = 0; i < m_pool_size.at(ObjName.ZOMBIE); ++i)
	{
		const auto zombie = std::make_shared<Zombie>(std::to_string(i));

		CreateObj(zombie);
		zombie->AddToObjManager();
	}
}

EnemyObjectPool::~EnemyObjectPool()
{
	DestroyObjects(ObjName.ZOMBIE);
}
