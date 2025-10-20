#include "screen_fader.hpp"

ScreenFader::ScreenFader(const int first_alpha_blend_num, const unsigned int fade_color) :
	m_fade_color					(fade_color),
	m_alpha_blend_num				(first_alpha_blend_num),
	m_destination_alpha_blend_num	(first_alpha_blend_num),
	m_fade_speed					(0.0f)
{

}

ScreenFader::~ScreenFader()
{

}

void ScreenFader::Update()
{
	if (m_destination_alpha_blend_num == m_alpha_blend_num) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_destination_alpha_blend_num > m_alpha_blend_num)
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(m_fade_speed * delta_time), m_destination_alpha_blend_num, false);
	}
	else
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(m_fade_speed * delta_time), m_destination_alpha_blend_num);
	}
}

void ScreenFader::Draw() const
{
	if (m_alpha_blend_num <= 0) { return; }

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha_blend_num);
	DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, kFadeColor, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ScreenFader::StartFade(const int destination_alpha_blend_num, const float fade_speed)
{
	m_destination_alpha_blend_num	= destination_alpha_blend_num;
	m_fade_speed					= fade_speed;
}
