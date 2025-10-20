#pragma once
#include <DxLib.h>

#include "../Base/singleton_base.hpp"
#include "../Window/window.hpp"
#include "../Calculation/math.hpp"

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

	[[nodiscard]] int	GetAlphaBlendNum()	const { return m_alpha_blend_num; }
	[[nodiscard]] bool	IsFading()			const { return m_alpha_blend_num != m_destination_alpha_blend_num; }

private:
	SceneFader();
	~SceneFader();

private:
	static constexpr unsigned int kFadeColor = 0x000000;

	int		m_alpha_blend_num;
	int		m_destination_alpha_blend_num;
	float	m_fade_speed;

	friend SingletonBase<SceneFader>;
};
