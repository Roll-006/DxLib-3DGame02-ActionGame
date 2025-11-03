#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/zombie.hpp"
#include "../Object/boss.hpp"

class EnemyObjectPool : public ObjectPoolBase
{
public:
	EnemyObjectPool();
	~EnemyObjectPool() override;

private:
	static constexpr int kZombiePoolSize	= 20;
	static constexpr int kBossPoolSize		= 1;

	int m_id_count;
};
