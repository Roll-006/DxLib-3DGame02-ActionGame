#include "scene_fader.hpp"

SceneFader::SceneFader() : 
	m_fader(std::make_unique<ScreenFader>(UCHAR_MAX, kFadeColor))
{

}

SceneFader::~SceneFader()
{

}

void SceneFader::Update()
{
	m_fader->Update();
}

void SceneFader::Draw() const
{
	m_fader->Draw();
}

void SceneFader::StartFade(const int destination_alpha_blend_num, const float fade_speed)
{
	m_fader->StartFade(destination_alpha_blend_num, fade_speed);
}
