#pragma once
#include "../Base/singleton_base.hpp"
#include "../Interface/i_observer.hpp"

#include "../ObjectPool/object_pool_holder.hpp"
#include "../ObjectPool/play_scene_effect_object_pool.hpp"
#include "../Event/event_system.hpp"

class GunBase;

class EffectManager final : public SingletonBase<EffectManager>
{
public:
	void Update();
	void LateUpdate();

	void Draw() const;

	/// @brief エフェクトを強制的にプールに戻す
	void ForciblyReturnPoolEffect(std::shared_ptr<Effect>& effect);
	void ForciblyReturnPoolEffect(const int return_trigger_handle, const std::string& object_pool_name);

private:
	EffectManager();
	~EffectManager();


	#pragma region Event
	/// @brief 武器が弾丸を発射した際のエフェクトを出力
	void OutputWeaponShotEffect			(const WeaponShotEvent& event);
	/// @brief ロケットランチャーが弾丸を発射した際のエフェクトを出力
	void OutputRocketLauncherShotEffect	(const RocketLauncherShotEvent& event);
	/// @brief 弾丸が発射された際のエフェクトを出力
	void OutputOnShotBulletEffect		(const OnShotBulletEvent& event);
	/// @brief 弾丸が衝突した際のエフェクトを出力
	void OutputOnHitBulletEffect		(const OnHitBulletEvent& event);
	/// @brief ダメージを受けた際のエフェクトを出力
	void OutputOnDamageEffect			(const OnDamageEvent& event);
	/// @brief タイトルシーンに遷移した際のエフェクトを出力
	void OutputTitleSceneEffect			(const OnChangeTitleSceneEvent& event);
	/// @brief 敵が消える際のエフェクトを出力
	void OutputDisappearEnemyEffect		(const StartDisappearEnemyEvent& event);
	/// @brief アイテムがドロップした際のエフェクトを出力
	void OutputDropItemEffect			(const DropItemEvent& event);
	#pragma endregion


	void AddEffect(const std::shared_ptr<Effect> effect);
	void ReturnPool();

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<ObjBase>>> m_effects;

	friend SingletonBase<EffectManager>;
};
