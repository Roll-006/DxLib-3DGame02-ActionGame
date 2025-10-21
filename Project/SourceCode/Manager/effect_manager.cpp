#include "effect_manager.hpp"
#include "../Base/gun_base.hpp"
#include "../Object/rocket_launcher.hpp"

EffectManager::EffectManager()
{
	// ÉCÉxÉìÉgìoò^
	EventSystem::GetInstance()->Subscribe<WeaponShotEvent>			(this, &EffectManager::OutputWeaponShotEffect);
	EventSystem::GetInstance()->Subscribe<RocketLauncherShotEvent>	(this, &EffectManager::OutputRocketLauncherShotEffect);
	EventSystem::GetInstance()->Subscribe<OnShotBulletEvent>		(this, &EffectManager::OutputOnShotBulletEffect);
	EventSystem::GetInstance()->Subscribe<OnHitBulletEvent>			(this, &EffectManager::OutputOnHitBulletEffect);
	EventSystem::GetInstance()->Subscribe<OnDamageEvent>			(this, &EffectManager::OutputOnDamageEffect);
	EventSystem::GetInstance()->Subscribe<OnChangeTitleSceneEvent>	(this, &EffectManager::OutputTitleSceneEffect);
}

EffectManager::~EffectManager()
{
	// ÉCÉxÉìÉgÇÃìoò^âèú
	EventSystem::GetInstance()->Unsubscribe<WeaponShotEvent>		(this, &EffectManager::OutputWeaponShotEffect);
	EventSystem::GetInstance()->Unsubscribe<RocketLauncherShotEvent>(this, &EffectManager::OutputRocketLauncherShotEffect);
	EventSystem::GetInstance()->Unsubscribe<OnShotBulletEvent>		(this, &EffectManager::OutputOnShotBulletEffect);
	EventSystem::GetInstance()->Unsubscribe<OnHitBulletEvent>		(this, &EffectManager::OutputOnHitBulletEffect);
	EventSystem::GetInstance()->Unsubscribe<OnDamageEvent>			(this, &EffectManager::OutputOnDamageEffect);
	EventSystem::GetInstance()->Unsubscribe<OnChangeTitleSceneEvent>(this, &EffectManager::OutputTitleSceneEffect);
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

void EffectManager::Draw() const
{
	for (const auto& effect : m_effects)
	{
		for (const auto& e : effect.second)
		{
			e->Draw();
		}
	}
}

void EffectManager::ForciblyReturnPoolEffect(std::shared_ptr<Effect>& effect)
{
	if (std::find(m_effects[effect->GetName()].begin(), m_effects[effect->GetName()].end(), effect) != m_effects[effect->GetName()].end())
	{
		StopEffekseer3DEffect(effect->GetPlayingEffectHandle());
		ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)->ReturnObj(effect);
		erase(m_effects[effect->GetName()], effect);
	}
}

void EffectManager::ForciblyReturnPoolEffect(const int return_trigger_handle, const std::string& object_pool_name)
{
	for (auto& [obj_name, objects] : m_effects)
	{
		auto& vec = objects;
		for (auto itr = vec.begin(); itr != vec.end();)
		{
			const auto effect = std::static_pointer_cast<Effect>(*itr);
			if (effect->GetReturnPoolTriggerHandle() == return_trigger_handle)
			{
				StopEffekseer3DEffect(effect->GetPlayingEffectHandle());
				ObjectPoolHolder::GetInstance()->GetObjectPool(object_pool_name)->ReturnObj(*itr);
				itr = vec.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}
}


#pragma region Event
void EffectManager::OutputWeaponShotEffect(const WeaponShotEvent& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	switch (event.gun_kind)
	{
	case GunKind::kSubmachineGun:
		obj = pool->GetObj(ObjName.SHOT_FIRE_EFFECT);
		if (obj)
		{
			const auto effect = std::static_pointer_cast<Effect>(obj);
			effect->AttachOwnerTransform(event.muzzle_transform);
			effect->SetOffsetAngle(VGet(90.0f * math::kDegToRad, 0.0f, 0.0f));
			effect->SetOffsetScale(2.5f);
			AddEffect(effect);
		}
		break;

	case GunKind::kRocketLauncher:
		obj = pool->GetObj(ObjName.EXPANDING_SMOKE_EFFECT);
		if (obj)
		{
			const auto effect = std::static_pointer_cast<Effect>(obj);
			effect->AttachOwnerTransform(event.muzzle_transform);
			effect->AddTimeScaleOwner(event.weapon_owner_name);
			effect->SetOffsetAngle(VGet(270.0f * math::kDegToRad, 0.0f, 0.0f));
			effect->SetOffsetScale(3.0f);
			AddEffect(effect);
		}
		break;

	default:
		break;
	}
}

void EffectManager::OutputRocketLauncherShotEffect(const RocketLauncherShotEvent& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	obj = pool->GetObj(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT);
	if (obj)
	{
		const auto effect = std::static_pointer_cast<Effect>(obj);
		effect->AttachOwnerTransform(event.ejection_port_transform);
		effect->AddTimeScaleOwner(event.weapon_owner_name);
		effect->SetOffsetAngle(VGet(270.0f * math::kDegToRad, 0.0f, 0.0f));
		effect->SetOffsetScale(1.0f);
		AddEffect(effect);
	}
}

void EffectManager::OutputOnShotBulletEffect(const OnShotBulletEvent& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	if (event.bullet_name == ObjName.ROCKET_BOMB)
	{
		obj = pool->GetObj(ObjName.ROCKET_BOMB_SMOKE_EFFECT);
		if (obj)
		{
			const auto effect = std::static_pointer_cast<Effect>(obj);
			effect->AttachOwnerTransform(event.bullet_transform);
			effect->AddTimeScaleOwner(event.weapon_owner_name);
			effect->AddReturnPoolTriggerHandle(event.obj_handle);
			effect->SetOffsetAngle(VGet(0.0f, DX_PI_F, 0.0f));
			effect->SetOffsetScale(2.5f);
			AddEffect(effect);
		}
	}
}

void EffectManager::OutputOnHitBulletEffect(const OnHitBulletEvent& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	if (event.bullet_name == ObjName.ROCKET_BOMB)
	{
		obj = pool->GetObj(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT);
		if (obj)
		{
			const auto effect = std::static_pointer_cast<Effect>(obj);
			effect->GetTransform()->SetPos(CoordinateKind::kWorld, event.hit_pos);
			effect->GetTransform()->SetRot(CoordinateKind::kWorld, event.move_dir);
			AddEffect(effect);
		}
	}

	//if (event.bullet_name == ObjName.BULLET)
	//{
	//	obj = pool->GetObj(ObjName.SHOT_FIRE_EFFECT);
	//	if (obj)
	//	{
	//		//const auto effect = std::static_pointer_cast<Effect>(obj);
	//		//effect->GetTransform()->SetPos(CoordinateKind::kWorld, event.hit_pos);
	//		////effect->GetTransform()->SetRot(CoordinateKind::kWorld, event.move_dir);
	//		//effect->SetOffsetAngle(VGet(270.0f * math::kDegToRad, 0.0f, 0.0f));
	//		//AddEffect(effect);
	//	}
	//}
}

void EffectManager::OutputOnDamageEffect(const OnDamageEvent& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	obj = pool->GetObj(ObjName.BLOOD_EFFECT);
	if (obj)
	{
		const auto effect = std::static_pointer_cast<Effect>(obj);
		effect->GetTransform()->SetPos(CoordinateKind::kWorld, event.hit_pos);
		AddEffect(effect);
	}
}

void EffectManager::OutputTitleSceneEffect(const OnChangeTitleSceneEvent& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.TITLE_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	obj = pool->GetObj(ObjName.TITLE_SMOKE);
	if (obj)
	{
		const auto effect = std::static_pointer_cast<Effect>(obj);
		effect->AttachOwnerTransform(event.smoke_transform);
		effect->AddReturnPoolTriggerHandle(event.smoke_delete_handle);
		effect->SetOffsetScale(2.5f);
		AddEffect(effect);
	}
}
#pragma endregion


void EffectManager::AddEffect(const std::shared_ptr<Effect> effect)
{
	if (std::find(m_effects[effect->GetName()].begin(), m_effects[effect->GetName()].end(), effect) == m_effects[effect->GetName()].end())
	{
		m_effects[effect->GetName()].emplace_back(effect);
	}
}
