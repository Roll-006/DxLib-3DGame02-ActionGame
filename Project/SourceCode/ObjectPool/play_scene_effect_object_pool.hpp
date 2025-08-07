#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/effect.hpp"

class PlaySceneEffectObjectPool : public ObjectPoolBase
{
public:
	PlaySceneEffectObjectPool();
	~PlaySceneEffectObjectPool() override;

private:
	static constexpr int kLightRocketLauncherExhaustVentPoolSize	= 3;
	static constexpr int kRocketBombSmokePoolSize					= 10;
	static constexpr int kExpandingSmokePoolSize					= 10;
};