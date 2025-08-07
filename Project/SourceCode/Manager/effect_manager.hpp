#pragma once
#include "../Base/singleton_base.hpp"
#include "../Interface/i_observer.hpp"

#include "../Part/subject.hpp"

#include "../ObjectPool/object_pool_holder.hpp"
#include "../ObjectPool/play_scene_effect_object_pool.hpp"
#include "../ObjectPool/share_scene_effect_object_pool.hpp"

class GunBase;

class EffectManager final : public SingletonBase<EffectManager>, public IObserver
{
public:
	void Update();
	void LateUpdate();

	template<typename T>
	void AddToSubject(const std::shared_ptr<Subject<T>> subject)
	{
		if (subject != nullptr)
		{
			// shared_ptrでラップして渡す（カスタムデリーター付き）
			std::shared_ptr<IObserver> observer = std::shared_ptr<IObserver>(this, [](IObserver*) {});
			subject->AddObserver(observer);
		}
	}

	void OnNotify(ObjBase& obj, const EventKind event_kind) override;

private:
	EffectManager();
	~EffectManager();

	void AddEffect(const std::shared_ptr<Effect> effect);


	#pragma region エフェクトの生成
	/// @brief 銃エフェクト
	void CreateGunEffect	(ObjBase& obj, const EventKind event_kind);
	/// @brief 弾丸エフェクト
	void CreateBulletEffect	(ObjBase& obj, const EventKind event_kind);
	#pragma endregion

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<Effect>>> m_effects;

	friend SingletonBase<EffectManager>;
};
