#include "play_scene_effect_object_pool.hpp"

PlaySceneEffectObjectPool::PlaySceneEffectObjectPool() :
	ObjectPoolBase(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)
{
	m_pool_size[ObjName.TEST] = kTestPoolSize;

	for (int i = 0; i < m_pool_size.at(ObjName.TEST); ++i)
	{
		const auto test = std::make_shared<Effect>(ObjName.TEST, 50.0f, false, EffectPath.TEST);
		test->GetTransform()->SetScale(CoordinateKind::kWorld, 10.0f);

		CreateObj(test);

		ObjManager::GetInstance()->AddObj(test);
	}
}

PlaySceneEffectObjectPool::~PlaySceneEffectObjectPool()
{

}
