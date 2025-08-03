#pragma once
#include "../Base/singleton_base.hpp"

#include "../ObjectPool/object_pool_holder.hpp"
#include "../ObjectPool/share_scene_effect_object_pool.hpp"
#include "../ObjectPool/play_scene_effect_object_pool.hpp"

class EffectManager final : public SingletonBase<EffectManager>
{
public:
	void Update();
	void LateUpdate();
	void Draw() const;

private:
	EffectManager();
	~EffectManager() override;

	void AddEffect(const std::shared_ptr<Effect> effect);

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<Effect>>> m_effects;

	friend SingletonBase<EffectManager>;
};
