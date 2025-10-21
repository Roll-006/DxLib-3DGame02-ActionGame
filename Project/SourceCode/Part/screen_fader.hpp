#pragma once
#include <DxLib.h>

#include "../Window/window.hpp"
#include "../Calculation/math.hpp"

class ScreenFader final
{
public:
	ScreenFader(const int first_alpha_blend_num, const unsigned int fade_color);
	~ScreenFader();

	void Update();
	void Draw() const;

	/// @brief フェードを開始する
	/// @param destination_alpha_blend_num 移行先アルファブレンド値 
	/// @param fade_speed フェード速度
	void StartFade(const int destination_alpha_blend_num, const float fade_speed);

	[[nodiscard]] int	GetAlphaBlendNum()				const { return m_alpha_blend_num; }
	[[nodiscard]] int	GetDestinationAlphaBlendNum()	const { return m_destination_alpha_blend_num; }
	[[nodiscard]] bool	IsFading()						const { return m_alpha_blend_num != m_destination_alpha_blend_num; }

private:
	static constexpr unsigned int kFadeColor = 0x000000;

	int		m_fade_color;
	int		m_alpha_blend_num;
	int		m_destination_alpha_blend_num;
	float	m_fade_speed;
};
