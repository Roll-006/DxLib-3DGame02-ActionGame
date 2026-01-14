#include "game_over_text.hpp"

GameOverText::GameOverText() :
	data				(GameOverTextData()),
	m_text_center_pos	(),
	m_alpha_blend_num	(0),
	m_scale				(1.0f),
	m_result_screen		(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/text_data.json", j_data))
	{
		data = j_data.at("text_data").at("game_over").get<GameOverTextData>();
	}

	data.text_data.font_handle	= FontHandler::GetInstance()->GetFontHandle(data.text_data.font_path);
	data.text_data.size			= { GetDrawStringWidthToHandle(data.text_data.text.c_str(), -1, data.text_data.font_handle), GetFontSizeToHandle(data.text_data.font_handle) };
	data.text_data.u_int_color	= type_converter::ConvertHEXToUINT(data.text_data.hex_color);

	m_result_screen		= std::make_shared<ScreenCreator>(data.screen_size, Vector2D<int>(Window::kCenterPos.x, static_cast<int>(Window::kScreenSize.y * data.height_ratio)));
	m_text_center_pos	= m_result_screen->GetHalfScreenSize();
}

GameOverText::~GameOverText()
{

}

void GameOverText::Init()
{

}

void GameOverText::Update()
{
	CreateResultScreen();
}

void GameOverText::Draw() const
{
	m_result_screen->Draw();
}

void GameOverText::CreateResultScreen()
{
	// 透過・スケールの設定
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_alpha_blend_num, static_cast<int>(100.0f * delta_time), UCHAR_MAX, false);
	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);

	m_result_screen->UseScreen();

	DrawStringToHandle(
		static_cast<int>((m_result_screen->GetScreenSize().x - data.text_data.size.x) * 0.5f),
		static_cast<int>((m_result_screen->GetScreenSize().y - data.text_data.size.y) * 0.5f),
		data.text_data.text.c_str(), data.text_data.u_int_color, data.text_data.font_handle);

	m_result_screen->UnuseScreen();
}
