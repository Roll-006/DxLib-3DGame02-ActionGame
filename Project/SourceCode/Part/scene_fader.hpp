#pragma once
#include <DxLib.h>

#include "../Base/singleton_base.hpp"
#include "screen_fader.hpp"

/// @brief シーン間のフェードアウト・フェードインを行う
class SceneFader final : public SingletonBase<SceneFader>
{
public:
	void Update();
	void Draw() const;

	/// @brief フェードを開始する
	/// @param destination_alpha_blend_num 移行先アルファブレンド値 
	/// @param fade_speed フェード速度
	void StartFade(const int destination_alpha_blend_num, const float fade_speed);

	[[nodiscard]] int	GetAlphaBlendNum()	const { return m_fader->GetAlphaBlendNum(); }
	[[nodiscard]] bool	IsFading()			const { return m_fader->IsFading(); }

private:
	SceneFader();
	~SceneFader();

private:
	static constexpr unsigned int kFadeColor = 0x000000;

	std::unique_ptr<ScreenFader> m_fader;

	friend SingletonBase<SceneFader>;
};
