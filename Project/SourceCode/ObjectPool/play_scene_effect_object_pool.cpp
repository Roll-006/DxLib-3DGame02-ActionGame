#include "play_scene_effect_object_pool.hpp"

PlaySceneEffectObjectPool::PlaySceneEffectObjectPool() :
	ObjectPoolBase(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)
{
	m_pool_size[ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT]	= kLightRocketLauncherExhaustVentPoolSize;
	m_pool_size[ObjName.EXPANDING_SMOKE_EFFECT]						= kExpandingSmokePoolSize;
	m_pool_size[ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT]			= kRocketBombHitExplosionPoolSize;
	m_pool_size[ObjName.ROCKET_BOMB_SMOKE_EFFECT]					= kRocketBombSmokePoolSize;

	// ロケットランチャーの排気口から出る光
	EffectData data = {	ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT, EffectPath.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT, 60.0f, 0.0f, false };
	for (int i = 0; i < m_pool_size.at(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// ロケット弾の煙
	data = { ObjName.ROCKET_BOMB_SMOKE_EFFECT, EffectPath.ROCKET_BOMB_SMOKE, 60.0f, 1.0f, true };
	for (int i = 0; i < m_pool_size.at(ObjName.ROCKET_BOMB_SMOKE_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// ロケット弾の着弾時爆発
	data = { ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT, EffectPath.ROCKET_BOMB_HIT_EXPLOSION, 30.0f, 0.0f, false };
	for (int i = 0; i < m_pool_size.at(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 広がる煙
	data = { ObjName.EXPANDING_SMOKE_EFFECT, EffectPath.EXPANDING_SMOKE, 40.0f, 0.0f, false };
	for (int i = 0; i < m_pool_size.at(ObjName.EXPANDING_SMOKE_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}
}

PlaySceneEffectObjectPool::~PlaySceneEffectObjectPool()
{

}
