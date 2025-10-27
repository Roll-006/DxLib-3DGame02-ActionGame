#pragma once
#include "../Base/physical_obj_base.hpp"
#include "../Interface/i_effect.hpp"
#include "../Interface/i_poolable.hpp"

#include "../GameTime/game_time_manager.hpp"
#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"

class CharacterBase;

class RocketBombExplosionEffect final : public PhysicalObjBase, public IEffect, public IPoolable
{
public:
	RocketBombExplosionEffect();
	~RocketBombExplosionEffect()	override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;


	#pragma region Attach / Detach
	void AttachOwnerTransform(const std::shared_ptr<Transform>& owner_transform) override;
	void DetachOwnerTransform() override;
	#pragma endregion


	#pragma region 登録 / 削除
	void AddTimeScaleOwner(const std::string& owner_name) override;
	void RemoveTimeScaleOwner() override;

	/// @brief 強制的にプールに返却させるためのハンドルを追加する
	/// @brief 主にオーナーとしてアタッチしたトランスフォームのオブジェクトハンドルを追加する
	void AddReturnPoolTriggerHandle(const int return_trigger_handle) override;
	void RemoveReturnPoolTriggerHandle() override;
	#pragma endregion


	#pragma region Setter
	void SetOffsetPos  (const VECTOR& offset_pos)   override { m_offset_pos	  = offset_pos; }
	void SetOffsetAngle(const VECTOR& offset_angle) override { m_offset_angle = offset_angle; }
	void SetOffsetScale(const VECTOR& offset_scale) override { m_offset_scale = offset_scale; }
	void SetOffsetScale(const float   offset_scale) override { m_offset_scale = VGet(offset_scale, offset_scale, offset_scale); }
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] float	GetDeltaTime()					const override;
	[[nodiscard]] int	GetOriginEffectHandle()			const override	{ return m_origin_effect_handle; }
	[[nodiscard]] int	GetPlayingEffectHandle()		const override	{ return m_playing_effect_handle; }
	[[nodiscard]] int   GetReturnPoolTriggerHandle()	const override	{ return m_return_pool_trigger_handle; }
	[[nodiscard]] float GetPower()						const			{ return kPower; }
	[[nodiscard]] bool	IsReturnPool() override;
	#pragma endregion

private:
	void ApplyMatrix()    const override;
	void ApplyPlaySpeed() const override;

	void PlayEffect() override;

	void CalcTriggerPos();
	void Attack(CharacterBase* target_character);

private:
	static constexpr float kScale			= 1.03f;
	static constexpr float kHitRadius		= 110.0f;
	static constexpr float kTriggerDeadTime = 1.5f;
	static constexpr float kPower			= 3000.0f;

	int			m_origin_effect_handle;			// エフェクトハンドル
	int			m_playing_effect_handle;		// 再生中のエフェクトハンドル

	int			m_return_pool_trigger_handle;	// エフェクトを強制的にプールに返却するためのトリガーとするハンドル
	std::string m_time_scale_owner_name;
	std::shared_ptr<Transform> m_owner_transform;

	VECTOR		m_offset_pos;
	VECTOR		m_offset_angle;
	VECTOR		m_offset_scale;

	EffectData	m_data;
	int			m_play_count;
	float		m_play_wait_timer;				// 再生開始を待つ時間を計測
	float		m_trigger_dead_timer;
};
