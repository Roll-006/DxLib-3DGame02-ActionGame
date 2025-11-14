#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/effect.hpp"
#include "../Object/rocket_bomb_explosion_effect.hpp"

class PlaySceneEffectObjectPool : public ObjectPoolBase
{
public:
	PlaySceneEffectObjectPool();
	~PlaySceneEffectObjectPool() override;

private:
	static constexpr int kLightRocketLauncherExhaustVentPoolSize	= 3;
	static constexpr int kRocketBombSmokePoolSize					= 10;
	static constexpr int kRocketBombHitExplosionPoolSize			= 5;
	static constexpr int kHitBulletLightPoolSize					= 10;
	static constexpr int kExpandingSmokePoolSize					= 10;
	static constexpr int kDisappearSmokePoolSize					= 10;
	static constexpr int kShotFirePoolSize							= 10;
	static constexpr int kBloodPoolSize								= 10;
	static constexpr int kItemRedPoolSize							= 10;
	static constexpr int kItemGreenPoolSize							= 10;
};
