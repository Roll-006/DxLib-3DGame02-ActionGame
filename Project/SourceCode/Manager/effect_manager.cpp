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

	// プールへ変換
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

void EffectManager::OnNotify(const IEvent& event)
{
	// 武器が弾丸を発射
	if (event.GetType() == std::type_index(typeid(WeaponShotData)))
	{
		const auto& weapon_shot	= static_cast<const Event<WeaponShotData>&>(event);
		OutputWeaponShotEffect(weapon_shot);
	}
	// ロケットランチャーが弾丸を発射
	if (event.GetType() == std::type_index(typeid(RocketLauncherShotData)))
	{
		const auto& rocket_launcher_shot = static_cast<const Event<RocketLauncherShotData>&>(event);
		OutputRocketLauncherShotEffect(rocket_launcher_shot);
	}
	// 弾丸が発射された
	if (event.GetType() == std::type_index(typeid(OnShotBulletData)))
	{
		const auto& on_shot_bullet = static_cast<const Event<OnShotBulletData>&>(event);
		OutputOnShotBulletEffect(on_shot_bullet);
	}
	// 弾丸が衝突した
	if (event.GetType() == std::type_index(typeid(OnHitBulletData)))
	{
		const auto& on_hit_bullet = static_cast<const Event<OnHitBulletData>&>(event);
		OutputOnHitBulletEffect(on_hit_bullet);
	}
}

void EffectManager::ForciblyReturnPoolEffect(std::shared_ptr<Effect> effect)
{
	if (std::find(m_effects[effect->GetName()].begin(), m_effects[effect->GetName()].end(), effect) != m_effects[effect->GetName()].end())
	{
		StopEffekseer3DEffect(effect->GetPlayingEffectHandle());
		ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL)->ReturnObj(effect);
		erase(m_effects[effect->GetName()], effect);
	}
}

void EffectManager::ForciblyReturnPoolEffect(const int return_trigger_handle)
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

void EffectManager::AddEffect(const std::shared_ptr<Effect> effect)
{
	if (std::find(m_effects[effect->GetName()].begin(), m_effects[effect->GetName()].end(), effect) == m_effects[effect->GetName()].end())
	{
		m_effects[effect->GetName()].emplace_back(effect);
	}
}

void EffectManager::OutputWeaponShotEffect(const Event<WeaponShotData>& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	switch (event.data.gun_kind)
	{
	case GunKind::kSniperRifle:
		break;

	case GunKind::kRocketLauncher:
		obj = pool->GetObj(ObjName.EXPANDING_SMOKE_EFFECT);
		if (obj)
		{
			const auto effect = std::static_pointer_cast<Effect>(obj);
			effect->AttachOwnerTransform(event.data.muzzle_transform);
			effect->AddTimeScaleOwner(event.data.weapon_owner_name);
			effect->SetOffsetAngle(VGet(270.0f * math::kDegToRad, 0.0f, 0.0f));
			effect->SetOffsetScale(3.0f);
			AddEffect(effect);
		}
		break;

	default:
		break;
	}
}

void EffectManager::OutputRocketLauncherShotEffect(const Event<RocketLauncherShotData>& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	obj = pool->GetObj(ObjName.LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT);
	if (obj)
	{
		const auto effect = std::static_pointer_cast<Effect>(obj);
		effect->AttachOwnerTransform(event.data.ejection_port_transform);
		effect->AddTimeScaleOwner(event.data.weapon_owner_name);
		effect->SetOffsetAngle(VGet(270.0f * math::kDegToRad, 0.0f, 0.0f));
		effect->SetOffsetScale(1.0f);
		AddEffect(effect);
	}
}

void EffectManager::OutputOnShotBulletEffect(const Event<OnShotBulletData>& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	if (event.data.bullet_name == ObjName.ROCKET_BOMB)
	{
		obj = pool->GetObj(ObjName.ROCKET_BOMB_SMOKE_EFFECT);
		if (obj)
		{
			const auto effect = std::static_pointer_cast<Effect>(obj);
			effect->AttachOwnerTransform(event.data.bullet_transform);
			effect->AddTimeScaleOwner(event.data.weapon_owner_name);
			effect->AddReturnPoolTriggerHandle(event.data.obj_handle);
			effect->SetOffsetAngle(VGet(0.0f, DX_PI_F, 0.0f));
			effect->SetOffsetScale(2.5f);
			AddEffect(effect);
		}
	}
}

void EffectManager::OutputOnHitBulletEffect(const Event<OnHitBulletData>& event)
{
	const auto pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
	std::shared_ptr<ObjBase> obj = nullptr;

	if (event.data.bullet_name == ObjName.ROCKET_BOMB)
	{
		obj = pool->GetObj(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT);
		if (obj)
		{
			const auto effect = std::static_pointer_cast<Effect>(obj);
			effect->GetTransform()->SetPos(CoordinateKind::kWorld, event.data.hit_pos);
			effect->GetTransform()->SetRot(CoordinateKind::kWorld, event.data.move_dir);
			AddEffect(effect);
		}
	}
}
