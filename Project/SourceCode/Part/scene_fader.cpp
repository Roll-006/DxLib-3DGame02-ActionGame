#include "scene_fader.hpp"

SceneFader::SceneFader() : 
	m_alpha_blend_num				(255),
	m_destination_alpha_blend_num	(255),
	m_fade_speed					(0.0f)
{

}

SceneFader::~SceneFader()
{

}

void SceneFader::Update()
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

void SceneFader::Draw() const
{
	if (m_alpha_blend_num <= 0) { return; }

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha_blend_num);
	DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, kFadeColor, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneFader::StartFade(const int destination_alpha_blend_num, const float fade_speed)
{
	m_destination_alpha_blend_num	= destination_alpha_blend_num;
	m_fade_speed					= fade_speed;
}
