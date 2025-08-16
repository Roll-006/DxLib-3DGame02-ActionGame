#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/bullet.hpp"
#include "../Object/shell_casing.hpp"
#include "../Object/rocket_bomb.hpp"
//#include "../Object/non_collide_rocket_bomb.hpp"

class RifleCartridgeObjectPool : public ObjectPoolBase
{
public:
	RifleCartridgeObjectPool();
	~RifleCartridgeObjectPool() override;

private:
	static constexpr int kBulletPoolSize				= 50;
	static constexpr int kShellCasingPoolSize			= 35;
	static constexpr int kRocketBombPoolSize			= 7;
	//static constexpr int kNonCollideRocketBombPoolSize	= 4;
};
