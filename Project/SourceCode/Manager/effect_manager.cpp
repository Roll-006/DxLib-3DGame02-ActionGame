#include "effect_manager.hpp"
#include "../Base/gun_base.hpp"
#include "../Object/rocket_launcher.hpp"

EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{

}

void EffectManager::Update()
{
	for (const auto& effect : m_effects)
	{
		for (const auto& e : effect.second)
		{
			e->Update();
		}
	}
}

void EffectManager::LateUpdate()
{
	for (const auto& effect : m_effects)
	{
		for (const auto& e : effect.second)
		{
			e->LateUpdate();
		}
	}

	Effekseer_Sync3DSetting();
	UpdateEffekseer3D();

	// ÉvÅ[ÉãÇ÷ïœä∑
	for (auto& [obj_name, objects] : m_effects)
	{
		auto& vec = objects;
		for (auto itr = vec.begin(); itr != vec.end();)
		{
			if (std::dynamic_pointer_cast<IPoolable>(*itr)->IsReturnPool())
			{
				ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)->ReturnObj(*itr);
				itr = vec.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}

void EffectManager::OnNotify(const IEvent& event)
{
	// ïêäÌÇ™íeä€Çî≠éÀ
	if (event.GetType() == std::type_index(typeid(WeaponShotData)))
	{
		const auto& weapon_shot		= static_cast<const Event<WeaponShotData>&>(event);
	}
	// íeä€Ç™î≠éÀÇ≥ÇÍÇΩ
	if (event.GetType() == std::type_index(typeid(OnShotBulletData)))
	{
		const auto& on_shot_bullet	= static_cast<const Event<OnShotBulletData>&>(event);
	}
	// íeä€Ç™è’ìÀÇµÇΩ
	if (event.GetType() == std::type_index(typeid(OnHitBulletData)))
	{
		const auto& on_hit_bullet	= static_cast<const Event<OnHitBulletData>&>(event);
	}
}

void EffectManager::AddEffect(const std::shared_ptr<Effect> effect)
{
	if (std::find(m_effects[effect->GetName()].begin(), m_effects[effect->GetName()].end(), effect) == m_effects[effect->GetName()].end())
	{
		m_effects[effect->GetName()].emplace_back(effect);
	}
}

void EffectManager::CreateGunEffect(ObjBase& obj)
{
	//const auto pool						= ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	//std::shared_ptr<ObjBase> effect_obj = nullptr;
	//std::shared_ptr<Effect>	 effect		= nullptr;
	//GunBase* gun						= dynamic_cast<GunBase*>(&obj);

	//if (gun)
	//{
	//	switch (gun->GetGunKind())
	//	{
	//	case GunKind::kSniperRifle:
	//		break;

	//	case GunKind::kRocketLauncher:
	//		effect_obj = pool->GetObj(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT);
	//		if (effect_obj)
	//		{
	//			effect = std::static_pointer_cast<Effect>(effect_obj);
	//			effect->AttachOwnerTransform(obj.GetTransform());
	//			//effect->SetOffset(VGet(0.0f, 0.0f, -60.0f), VGet(270.0f * math::kDegreesToRadian, 0.0f, 0.0f), 1.5f);
	//			AddEffect(effect);
	//		}

	//		effect_obj = pool->GetObj(ObjName.EXPANDING_SMOKE_EFFECT);
	//		if (effect_obj)
	//		{
	//			effect = std::static_pointer_cast<Effect>(effect_obj);
	//			effect->AttachOwnerTransform(obj.GetTransform());
	//			//effect->SetOffset(VGet(0.0f, 0.0f, 70.0f), VGet(270.0f * math::kDegreesToRadian, 0.0f, 0.0f), 3.0f);
	//			AddEffect(effect);
	//		}
	//		break;

	//	default:
	//		break;
	//	}

	//}
}

void EffectManager::CreateBulletEffect(ObjBase& obj)
{
	//const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	//std::shared_ptr<ObjBase> effect_obj = nullptr;
	//std::shared_ptr<Effect>	 effect		= nullptr;

	//if (obj.GetTag() != ObjTag.BULLET) { return; }

	//if (obj.GetName() == ObjName.ROCKET_BOMB)
	//{
	//	effect_obj = pool->GetObj(ObjName.ROCKET_BOMB_SMOKE_EFFECT);
	//	if (effect_obj)
	//	{
	//		effect = std::static_pointer_cast<Effect>(effect_obj);
	//		effect->AttachOwnerTransform(obj.GetTransform());
	//		effect->SetOffset(VGet(0.0f, 0.0f, 0.0f), VGet(0.0f, DX_PI_F, 0.0f), 2.5f);
	//		AddEffect(effect);
	//	}
	//}
}
