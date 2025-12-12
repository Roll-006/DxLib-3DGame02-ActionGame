#include "title_scene_effect_object_pool.hpp"

TitleSceneEffectObjectPool::TitleSceneEffectObjectPool() :
	ObjectPoolBase(ObjectPoolName.TITLE_SCENE_EFFECT_POOL)
{
	m_pool_size[ObjName.TITLE_SMOKE] = kSmokePoolSize;

	// ‰Œ
	EffectData j_data = {	ObjName.TITLE_SMOKE, EffectPath.TITLE_SMOKE, 10.0f, 0.0f, true };
	for (int i = 0; i < m_pool_size.at(j_data.obj_name); ++i)
	{
		const auto effect = std::make_shared<Effect>(j_data);
		CreateObj(effect);
		effect->AddToObjManager();
	}
}

TitleSceneEffectObjectPool::~TitleSceneEffectObjectPool()
{
	DestroyObjects(ObjName.TITLE_SMOKE);
}
