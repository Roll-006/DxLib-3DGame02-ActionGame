#include "play_scene_effect_object_pool.hpp"

PlaySceneEffectObjectPool::PlaySceneEffectObjectPool() :
	ObjectPoolBase(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)
{
	m_pool_size[ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT]	= kLightRocketLauncherExhaustVentPoolSize;
	m_pool_size[ObjName.EXPANDING_SMOKE_EFFECT]						= kExpandingSmokePoolSize;
	m_pool_size[ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT]			= kRocketBombHitExplosionPoolSize;
	m_pool_size[ObjName.ROCKET_BOMB_SMOKE_EFFECT]					= kRocketBombSmokePoolSize;
	m_pool_size[ObjName.SHOT_FIRE_EFFECT]							= kShotFirePoolSize;
	m_pool_size[ObjName.BLOOD_EFFECT]								= kBloodPoolSize;

	// ロケットランチャーの排気口から出る光
	EffectData data = {	ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT, EffectPath.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT, 60.0f, 0.0f, false };
	for (int i = 0; i < m_pool_size.at(data.obj_name); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// ロケット弾の煙
	data = { ObjName.ROCKET_BOMB_SMOKE_EFFECT, EffectPath.ROCKET_BOMB_SMOKE, 60.0f, 1.0f, true };
	for (int i = 0; i < m_pool_size.at(data.obj_name); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// ロケット弾の着弾時爆発
	for (int i = 0; i < m_pool_size.at(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT); ++i)
	{
		const auto effect = std::make_shared<RocketBombExplosionEffect>();
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 広がる煙
	data = { ObjName.EXPANDING_SMOKE_EFFECT, EffectPath.EXPANDING_SMOKE, 40.0f, 0.0f, false };
	for (int i = 0; i < m_pool_size.at(data.obj_name); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 弾丸発射時の炎
	data = { ObjName.SHOT_FIRE_EFFECT, EffectPath.SHOT_FIRE, 60.0f, 0.0f, false };
	for (int i = 0; i < m_pool_size.at(data.obj_name); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 血
	data = { ObjName.BLOOD_EFFECT, EffectPath.BLOOD, 70.0f, 0.0f, false };
	for (int i = 0; i < m_pool_size.at(data.obj_name); ++i)
	{
		const auto effect = std::make_shared<Effect>(data);
		CreateObj(effect);
		effect->AddToObjManager();
	}
}

PlaySceneEffectObjectPool::~PlaySceneEffectObjectPool()
{
	DestroyObjects(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT);
	DestroyObjects(ObjName.EXPANDING_SMOKE_EFFECT);
	DestroyObjects(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT);
	DestroyObjects(ObjName.ROCKET_BOMB_SMOKE_EFFECT);
	DestroyObjects(ObjName.SHOT_FIRE_EFFECT);
	DestroyObjects(ObjName.BLOOD_EFFECT);
}
