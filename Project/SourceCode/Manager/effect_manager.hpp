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

	void OnNotify(const IEvent& event) override;

	/// @brief エフェクトを強制的にプールに戻す
	void ForciblyReturnPoolEffect(std::shared_ptr<Effect> effect);
	void ForciblyReturnPoolEffect(const int return_trigger_handle);

private:
	EffectManager();
	~EffectManager();

	void AddEffect(const std::shared_ptr<Effect> effect);


	#pragma region エフェクトの出力
	/// @brief 武器が弾丸を発射した際のエフェクトを出力
	void OutputWeaponShotEffect			(const Event<WeaponShotData>& event);
	/// @brief ロケットランチャーが弾丸を発射した際のエフェクトを出力
	void OutputRocketLauncherShotEffect	(const Event<RocketLauncherShotData>& event);
	/// @brief 弾丸が発射された際のエフェクトを出力
	void OutputOnShotBulletEffect		(const Event<OnShotBulletData>& event);
	/// @brief 弾丸が衝突した際のエフェクトを出力
	void OutputOnHitBulletEffect		(const Event<OnHitBulletData>& event);
	#pragma endregion

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<Effect>>> m_effects;

	friend SingletonBase<EffectManager>;
};
