#include "game_clear_text.hpp"

GameClearText::GameClearText() : 
	m_font_handle		(FontHandler::GetInstance()->GetFontHandle(FontName.GAME_CLEAR_TEXT)),
	m_text				("MISSION COMPLETE"),
	m_font_size			(GetDrawStringWidthToHandle(m_text.c_str(), -1, m_font_handle), GetFontSizeToHandle(m_font_handle)),
	m_alpha_blend_num	(0),
	m_scale				(0.5f),
	m_result_screen		(std::make_shared<ScreenCreator>(kScreenSize, kCenterPos))
{

}

GameClearText::~GameClearText()
{

}

void GameClearText::Init()
{

}

void GameClearText::Update()
{
	CreateResultScreen();
}

void GameClearText::Draw() const
{
	m_result_screen->Draw();
}

void GameClearText::CreateResultScreen()
{
	// 透過・スケールの設定
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_alpha_blend_num, static_cast<int>(200.0f * delta_time), UCHAR_MAX, false);
	math::Increase(m_scale, 2.0f * delta_time, 1.0f, false);
	m_result_screen->GetGraphicer()->SetScale	(m_scale);
	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);

	m_result_screen->UseScreen();

	DrawStringToHandle(
		static_cast<int>((m_result_screen->GetScreenSize().x - m_font_size.x) * 0.5f),
		static_cast<int>((m_result_screen->GetScreenSize().y - m_font_size.y) * 0.5f),
		m_text.c_str(), 0xffffff, m_font_handle);

	m_result_screen->UnuseScreen();
}
