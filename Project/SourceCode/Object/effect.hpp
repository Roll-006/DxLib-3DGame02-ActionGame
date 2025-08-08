#pragma once
#include "../Base/obj_base.hpp"
#include "../Interface/i_poolable.hpp"

#include "../Manager/obj_manager.hpp"

#include "../Data/Path/effect_path.hpp"
#include "../Data/effect_data.hpp"
#include "../Handle/handle_keeper.hpp"

class Effect final : public ObjBase, public IPoolable
{
public:
	Effect(const EffectData& data);
	~Effect() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void AddToObjManager()			override;

	void AttachOwnerTransform(const std::shared_ptr<Transform> owner_transform);
	void DetachOwnerTransform();

	void SetOffsetPos  (const VECTOR& offset_pos)   { m_offset_pos		= offset_pos; }
	void SetOffsetAngle(const VECTOR& offset_angle) { m_offset_angle	= offset_angle; }
	void SetOffsetScale(const VECTOR& offset_scale) { m_offset_scale	= offset_scale; }
	void SetOffsetScale(const float   offset_scale) { m_offset_scale	= VGet(offset_scale, offset_scale, offset_scale); }

	[[nodiscard]] int	GetOriginEffectHandle()	 const	{ return m_origin_effect_handle; }
	[[nodiscard]] int	GetPlayingEffectHandle() const	{ return m_playing_effect_handle; }
	[[nodiscard]] bool	IsReturnPool() override;

private:
	void ApplyMatrix()    const;
	void ApplyPlaySpeed() const;

	void PlayEffect();

private:
	int   m_origin_effect_handle;		// エフェクトハンドル
	int   m_playing_effect_handle;		// 再生中のエフェクトハンドル

	std::shared_ptr<Transform> m_owner_transform;

	VECTOR		m_offset_pos;
	VECTOR		m_offset_angle;
	VECTOR		m_offset_scale;

	EffectData	m_data;
	int			m_play_count;
	float		m_play_wait_timer;			// 再生開始を待つ時間を計測
};