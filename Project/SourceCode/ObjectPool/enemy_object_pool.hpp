#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/zombie.hpp"

class EnemyObjectPool : public ObjectPoolBase
{
public:
	EnemyObjectPool();
	~EnemyObjectPool() override;

private:
	static constexpr int kPoolSize = 20;
};
