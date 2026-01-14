#pragma once
#include "../Base/obj_base.hpp"
#include "../Interface/i_effect.hpp"
#include "../Interface/i_poolable.hpp"

#include "../GameTime/game_time_manager.hpp"
#include "../Accessor/obj_accessor.hpp"

class Effect final : public ObjBase, public IEffect, public IPoolable
{
public:
	Effect(const EffectData& j_data);
	~Effect() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

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
	void SetOffsetPos  (const VECTOR& offset_pos)   override { m_offset_pos	 = offset_pos; }
	void SetOffsetAngle(const VECTOR& offset_angle) override { m_offset_angle = offset_angle; }
	void SetOffsetScale(const VECTOR& offset_scale) override { m_offset_scale = offset_scale; }
	void SetOffsetScale(const float   offset_scale) override { m_offset_scale = VGet(offset_scale, offset_scale, offset_scale); }
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] const int		GetOriginEffectHandle()		 const override { return m_origin_effect_handle; }
	[[nodiscard]] const int		GetPlayingEffectHandle()	 const override { return m_playing_effect_handle; }
	[[nodiscard]] const int		GetReturnPoolTriggerHandle() const override { return m_return_pool_trigger_handle; }
	[[nodiscard]] const bool	IsReturnPool() override;
	#pragma endregion

private:
	void ApplyMatrix()    const override;
	void ApplyPlaySpeed() const override;

	void PlayEffect() override;

	[[nodiscard]] const float GetDeltaTime() const override;

private:
	int			m_origin_effect_handle;			// エフェクトハンドル
	int			m_playing_effect_handle;		// 再生中のエフェクトハンドル

	int			m_return_pool_trigger_handle;	// エフェクトを強制的にプールに返却するためのトリガーとするハンドル
	std::string m_time_scale_owner_name;
	std::shared_ptr<Transform> m_owner_transform;

	VECTOR		m_offset_pos;
	VECTOR		m_offset_angle;
	VECTOR		m_offset_scale;

	EffectData	m_data;
	int			m_play_contains;
	float		m_play_wait_timer;				// 再生開始を待つ時間を計測
};
