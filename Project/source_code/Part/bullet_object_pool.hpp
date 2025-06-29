#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/bullet.hpp"

class BulletObjectPool : public ObjectPoolBase
{
public:
	BulletObjectPool();
	~BulletObjectPool() override;

private:
	static constexpr int kBulletPoolSize = 30;
};