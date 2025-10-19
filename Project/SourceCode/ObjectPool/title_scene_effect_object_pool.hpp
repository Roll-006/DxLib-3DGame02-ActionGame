#pragma once
#include "../Base/object_pool_base.hpp"
#include "../Object/effect.hpp"

class TitleSceneEffectObjectPool : public ObjectPoolBase
{
public:
	TitleSceneEffectObjectPool();
	~TitleSceneEffectObjectPool() override;

private:
	static constexpr int kSmokePoolSize	= 20;
};
