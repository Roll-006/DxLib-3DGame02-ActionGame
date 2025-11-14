#include "play_scene_effect_object_pool.hpp"

PlaySceneEffectObjectPool::PlaySceneEffectObjectPool() :
	ObjectPoolBase(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)
{
	m_pool_size[ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT]	= kLightRocketLauncherExhaustVentPoolSize;
	m_pool_size[ObjName.EXPANDING_SMOKE_EFFECT]						= kExpandingSmokePoolSize;
	m_pool_size[ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT]			= kRocketBombHitExplosionPoolSize;
	m_pool_size[ObjName.HIT_BULLET_LIGHT_EFFECT]					= kHitBulletLightPoolSize;
	m_pool_size[ObjName.ROCKET_BOMB_SMOKE_EFFECT]					= kRocketBombSmokePoolSize;
	m_pool_size[ObjName.DISAPPEAR_SMOKE_EFFECT]						= kDisappearSmokePoolSize;
	m_pool_size[ObjName.SHOT_FIRE_EFFECT]							= kShotFirePoolSize;
	m_pool_size[ObjName.BLOOD_EFFECT]								= kBloodPoolSize;
	m_pool_size[ObjName.ITEM_RED_EFFECT]							= kItemRedPoolSize;
	m_pool_size[ObjName.ITEM_GREEN_EFFECT]							= kItemGreenPoolSize;

	// ロケットランチャーの排気口から出る光
	for (int i = 0; i < m_pool_size.at(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT, EffectPath.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT, 60.0f, 0.0f, false));
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// ロケット弾の煙
	for (int i = 0; i < m_pool_size.at(ObjName.ROCKET_BOMB_SMOKE_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.ROCKET_BOMB_SMOKE_EFFECT, EffectPath.ROCKET_BOMB_SMOKE, 60.0f, 1.0f, true));
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

	// 弾丸着弾時の光
	for (int i = 0; i < m_pool_size.at(ObjName.HIT_BULLET_LIGHT_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.HIT_BULLET_LIGHT_EFFECT, EffectPath.HIT_BULLET_LIGHT, 70.0f, 0.0f, false));
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 広がる煙
	for (int i = 0; i < m_pool_size.at(ObjName.EXPANDING_SMOKE_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.EXPANDING_SMOKE_EFFECT, EffectPath.EXPANDING_SMOKE, 40.0f, 0.0f, false));
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 敵が消える際の煙
	for (int i = 0; i < m_pool_size.at(ObjName.DISAPPEAR_SMOKE_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.DISAPPEAR_SMOKE_EFFECT, EffectPath.DISAPPEAR_SMOKE, 40.0f, 0.0f, false));
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 弾丸発射時の炎
	for (int i = 0; i < m_pool_size.at(ObjName.SHOT_FIRE_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.SHOT_FIRE_EFFECT, EffectPath.SHOT_FIRE, 60.0f, 0.0f, false));
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// 血
	for (int i = 0; i < m_pool_size.at(ObjName.BLOOD_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.BLOOD_EFFECT, EffectPath.BLOOD, 70.0f, 0.0f, false));
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// アイテム(弾薬)
	for (int i = 0; i < m_pool_size.at(ObjName.ITEM_RED_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.ITEM_RED_EFFECT, EffectPath.ITEM_RED, 70.0f, 0.0f, true));
		CreateObj(effect);
		effect->AddToObjManager();
	}

	// アイテム(回復)
	for (int i = 0; i < m_pool_size.at(ObjName.ITEM_GREEN_EFFECT); ++i)
	{
		const auto effect = std::make_shared<Effect>(EffectData(ObjName.ITEM_GREEN_EFFECT, EffectPath.ITEM_GREEN, 70.0f, 0.0f, true));
		CreateObj(effect);
		effect->AddToObjManager();
	}
}

PlaySceneEffectObjectPool::~PlaySceneEffectObjectPool()
{
	DestroyObjects(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT);
	DestroyObjects(ObjName.EXPANDING_SMOKE_EFFECT);
	DestroyObjects(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT);
	DestroyObjects(ObjName.HIT_BULLET_LIGHT_EFFECT);
	DestroyObjects(ObjName.ROCKET_BOMB_SMOKE_EFFECT);
	DestroyObjects(ObjName.DISAPPEAR_SMOKE_EFFECT);
	DestroyObjects(ObjName.SHOT_FIRE_EFFECT);
	DestroyObjects(ObjName.BLOOD_EFFECT);
	DestroyObjects(ObjName.ITEM_RED_EFFECT);
	DestroyObjects(ObjName.ITEM_GREEN_EFFECT);
}
