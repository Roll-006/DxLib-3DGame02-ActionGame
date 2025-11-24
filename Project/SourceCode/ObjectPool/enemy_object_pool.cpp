#include "enemy_object_pool.hpp"

EnemyObjectPool::EnemyObjectPool() :
	ObjectPoolBase	(ObjectPoolName.ENEMY_POOL),
	m_id_count		(0)
{
	m_pool_size[ObjName.ZOMBIE] = kZombiePoolSize;

	// ゾンビ
	for (size_t i = 0; m_pool_size.at(ObjName.ZOMBIE) < 1; ++i)
	{
		const auto id		= "zombie" + std::to_string(i);
		const auto zombie	= std::make_shared<Zombie>(id);

		CreateObj(zombie);
		zombie->AddToObjManager();

		const auto obj = ObjManager::GetInstance()->GetObj<Zombie>(ObjName.ZOMBIE).use_count();
	}
}

EnemyObjectPool::~EnemyObjectPool()
{
	auto obj = ObjManager::GetInstance()->GetObj<Zombie>(ObjName.ZOMBIE).use_count();

	DestroyObjects(ObjName.ZOMBIE);

	obj = ObjManager::GetInstance()->GetObj<Zombie>(ObjName.ZOMBIE).use_count();
}
