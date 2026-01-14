#include "main_menu_select_button.hpp"

MainMenuSelectButton::MainMenuSelectButton(const TextData& text_data, const Vector2D<int>& center_pos, const std::function<void()> exeute_function, const bool is_init_selected) :
	UIButtonBase				(exeute_function),
	data						(MainMenuSelectButtonData()),
	m_text_data					(text_data),
	m_alpha_blend_num			(UCHAR_MAX),
	m_current_graphic_scale		(0.0f),
	m_destination_graphic_scale	(0.0f),
	m_button_screen				(nullptr)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/main_menu_select_button_data.json", j_data))
	{
		data = j_data.at("main_menu_select_button_data").get<MainMenuSelectButtonData>();
	}

	m_alpha_blend_num			= data.non_select_alpha_blend_num;
	m_current_graphic_scale		= m_destination_graphic_scale = data.non_select_scale;

	// É{É^ÉìÇÃâÊëúÇê∂ê¨
	const auto screen_height = m_text_data.size.y + data.screen_height_offset;
	m_button_screen = std::make_shared<ScreenCreator>(Vector2D<int>(data.screen_width, screen_height), center_pos);
	m_button_screen->UseScreen();
	DrawStringToHandle(
		data.screen_height_offset,
		static_cast<int>((screen_height - m_text_data.size.y) * 0.5f),
		m_text_data.text.c_str(), m_text_data.u_int_color, m_text_data.font_handle);
	m_button_screen->UnuseScreen();

	// ç≈èâÇ©ÇÁëIëÇ≥ÇÍÇƒÇ¢ÇÈèÍçáÇÃèàóù
	if (is_init_selected)
	{
		m_alpha_blend_num		= UCHAR_MAX;
		m_current_graphic_scale = data.select_scale;
	}

	CalcAlphaBlendNum();
	CalcGraphicScale();
}

MainMenuSelectButton::~MainMenuSelectButton()
{

}

void MainMenuSelectButton::Init()
{

}

void MainMenuSelectButton::Update()
{
	CalcAlphaBlendNum();
	CalcGraphicScale();

	Exeute();
}

void MainMenuSelectButton::Draw() const
{
	m_button_screen->Draw();
}

void MainMenuSelectButton::CalcAlphaBlendNum()
{
	m_alpha_blend_num = m_is_active ? UCHAR_MAX : data.non_select_alpha_blend_num;
	m_button_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void MainMenuSelectButton::CalcGraphicScale()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_is_active)
	{
		m_destination_graphic_scale = data.select_scale;
		math::Increase(m_current_graphic_scale, data.fade_in_speed  * delta_time, m_destination_graphic_scale, false);
	}
	else
	{
		m_destination_graphic_scale = data.non_select_scale;
		math::Decrease(m_current_graphic_scale, data.fade_out_speed * delta_time, m_destination_graphic_scale);
	}

	m_button_screen->GetGraphicer()->SetScale(m_current_graphic_scale);
}
