#include "enemy_object_pool.hpp"

EnemyObjectPool::EnemyObjectPool() :
	ObjectPoolBase	(ObjectPoolName.ENEMY_POOL),
	m_id_contains	(0)
{
	m_pool_size[ObjName.ZOMBIE] = kZombiePoolSize;

	// ゾンビ
	for (size_t i = 0; i < m_pool_size.at(ObjName.ZOMBIE); ++i)
	{
		// 仮分岐
		auto name = "";
		if (i < 3)
		{
			name = "police";
		}
		else if(i < 5)
		{
			name = "woman";
		}
		else
		{
			name = "parasitic";
		}

		const auto id		= "zombie" + std::to_string(i);
		const auto zombie	= std::make_shared<Zombie>(id, name);

		CreateObj(zombie);
		zombie->AddToObjManager();
	}
}

EnemyObjectPool::~EnemyObjectPool()
{
	DestroyObjects(ObjName.ZOMBIE);
}
