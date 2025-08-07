#include "play_scene_effect_object_pool.hpp"

PlaySceneEffectObjectPool::PlaySceneEffectObjectPool() :
	ObjectPoolBase(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)
{
	m_pool_size[ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT] = kLightRocketLauncherExhaustVentEffectPoolSize;

	for (int i = 0; i < m_pool_size.at(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT, 50.0f, false, EffectPath.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT);
		effect->GetTransform()->SetScale(CoordinateKind::kWorld, 10.0f);

		CreateObj(effect);
		effect->AddToObjManager();
	}
}

PlaySceneEffectObjectPool::~PlaySceneEffectObjectPool()
{

}
