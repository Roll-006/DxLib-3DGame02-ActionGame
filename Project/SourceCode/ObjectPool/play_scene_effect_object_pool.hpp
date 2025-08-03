#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/effect.hpp"

class PlaySceneEffectObjectPool : public ObjectPoolBase
{
public:
	PlaySceneEffectObjectPool();
	~PlaySceneEffectObjectPool() override;

private:
	static constexpr int kTestPoolSize = 10;
};