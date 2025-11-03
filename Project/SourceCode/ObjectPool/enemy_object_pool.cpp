#include "enemy_object_pool.hpp"

EnemyObjectPool::EnemyObjectPool() :
	ObjectPoolBase	(ObjectPoolName.ENEMY_POOL),
	m_id_count		(0)
{
	m_pool_size[ObjName.ZOMBIE] = kZombiePoolSize;
	m_pool_size[ObjName.BOSS]	= kBossPoolSize;

	// ゾンビ
	for (size_t i = 0; i < m_pool_size.at(ObjName.ZOMBIE); ++i)
	{
		const auto id		= "zombie" + std::to_string(i);
		const auto zombie	= std::make_shared<Zombie>(id);

		CreateObj(zombie);
		zombie->AddToObjManager();
	}

	// ボス
	for (size_t i = 0; i < m_pool_size.at(ObjName.BOSS); ++i)
	{
		const auto id = "boss" + std::to_string(i);
		const auto boss = std::make_shared<Boss>(id);

		CreateObj(boss);
		boss->AddToObjManager();
	}
}

EnemyObjectPool::~EnemyObjectPool()
{
	DestroyObjects(ObjName.ZOMBIE);
	DestroyObjects(ObjName.BOSS);
}
