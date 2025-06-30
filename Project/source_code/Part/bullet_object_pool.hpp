#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/bullet.hpp"
#include "../Object/shell_casing.hpp"

class BulletObjectPool : public ObjectPoolBase
{
public:
	BulletObjectPool();
	~BulletObjectPool() override;

private:
	static constexpr int kBulletPoolSize		= 20;
	static constexpr int kShellCasingPoolSize	= 30;
};