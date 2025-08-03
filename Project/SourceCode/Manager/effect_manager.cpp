#include "effect_manager.hpp"



#include "../Input/input_checker.hpp"

EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{

}

void EffectManager::Update()
{
	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_G) == InputState::kSingle)
	{
		const auto object_pool = ObjectPoolHolder::GetInstance()->GetObjectPool(ObjectPoolName.PLAY_SCENE_EFFECT_POOL);

		// プールから弾丸を取得し、有効であれば発射
		const auto effect = std::static_pointer_cast<Effect>(object_pool->GetObj(ObjName.TEST));
		if (effect != nullptr)
		{
			AddEffect(effect);
		}
	}

	// TODO : オブザーバーにてエフェクトを呼び出す

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
	DrawEffekseer3D();
}

void EffectManager::AddEffect(const std::shared_ptr<Effect> effect)
{
	if (std::find(m_effects[effect->GetName()].begin(), m_effects[effect->GetName()].end(), effect) == m_effects[effect->GetName()].end())
	{
		m_effects[effect->GetName()].emplace_back(effect);
	}
}
