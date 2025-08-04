#pragma once
#include "../Base/obj_base.hpp"
#include "../Interface/i_poolable.hpp"

#include "../Manager/obj_manager.hpp"

#include "../Data/Path/effect_path.hpp"
#include "../Handle/handle_keeper.hpp"

class Effect final : public ObjBase, public IPoolable
{
public:
	Effect(const std::string& obj_name, const float play_speed, const bool is_loop, const std::string& file_path);
	~Effect() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

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
	int   m_play_count;
	float m_play_speed;
	bool  m_is_loop;
};