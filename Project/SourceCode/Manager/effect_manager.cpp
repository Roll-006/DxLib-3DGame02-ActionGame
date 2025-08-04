#include "effect_manager.hpp"

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

	// ƒv[ƒ‹‚Ö•ÏŠ·
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

void EffectManager::OnNotify(const std::shared_ptr<ObjBase> obj, const EventKind event_kind)
{
	switch (event_kind)
	{
	case EventKind::k:
		break;

	default:
		break;
	}
}

void EffectManager::AddEffect(const std::shared_ptr<Effect> effect)
{
	if (std::find(m_effects[effect->GetName()].begin(), m_effects[effect->GetName()].end(), effect) == m_effects[effect->GetName()].end())
	{
		m_effects[effect->GetName()].emplace_back(effect);
	}
}
