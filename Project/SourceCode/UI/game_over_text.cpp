#include "game_over_text.hpp"

GameOverText::GameOverText() :
	game_over_text_data	(),
	m_text_center_pos	(),
	m_alpha_blend_num	(0),
	m_scale				(1.0f),
	m_result_screen		(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/text_data.json", j_data))
	{
		game_over_text_data = j_data.at("text_data").at("game_over").get<GameOverTextData>();

		game_over_text_data.text_data.font_handle	= FontHandler::GetInstance()->GetFontHandle(game_over_text_data.text_data.font_path);
		game_over_text_data.text_data.font_size		= { GetDrawStringWidthToHandle(game_over_text_data.text_data.text.c_str(), -1, game_over_text_data.text_data.font_handle), GetFontSizeToHandle(game_over_text_data.text_data.font_handle) };
		game_over_text_data.text_data.u_int_color	= type_converter::ConvertHEXToUINT(game_over_text_data.text_data.hex_color);

		m_result_screen		= std::make_shared<ScreenCreator>(game_over_text_data.screen_size, Vector2D<int>(Window::kCenterPos.x, static_cast<int>(Window::kScreenSize.y * game_over_text_data.height_ratio)));
		m_text_center_pos	= m_result_screen->GetHalfScreenSize();
	}
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
		static_cast<int>((m_result_screen->GetScreenSize().x - game_over_text_data.text_data.font_size.x) * 0.5f),
		static_cast<int>((m_result_screen->GetScreenSize().y - game_over_text_data.text_data.font_size.y) * 0.5f),
		game_over_text_data.text_data.text.c_str(), game_over_text_data.text_data.u_int_color, game_over_text_data.text_data.font_handle);

	m_result_screen->UnuseScreen();
}
