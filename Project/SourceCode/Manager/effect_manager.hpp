#pragma once
#include "../Base/one_instance_singleton_base.hpp"
#include "../Interface/i_observer.hpp"

#include "../ObjectPool/object_pool_holder.hpp"
#include "../ObjectPool/play_scene_effect_object_pool.hpp"
#include "../ObjectPool/share_scene_effect_object_pool.hpp"

class EffectManager final : public OneInstanceSingletonBase<EffectManager>, public IObserver
{
public:
	EffectManager();
	~EffectManager();

	void Update();
	void LateUpdate();

	void OnNotify(const std::shared_ptr<ObjBase> obj, const EventKind event_kind) override;

private:
	void AddEffect(const std::shared_ptr<Effect> effect);

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<Effect>>> m_effects;

	friend OneInstanceSingletonBase<EffectManager>;
};
