#include "mission_ui.hpp"

MissionUI::MissionUI() : 
	mission_ui_data		(MissionUIData()),
	m_screen			(nullptr),
	m_text_center_pos	(),
	m_alpha_blend_num	(0),
	m_wait_timer		(0.0f),
	m_is_active			(false),
	m_is_wait			(true)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/text_data.json", j_data))
	{
		mission_ui_data = j_data.at("text_data").at("mission").get<MissionUIData>();

		mission_ui_data.text_data.font_handle	= FontHandler::GetInstance()->GetFontHandle(mission_ui_data.text_data.font_path);
		mission_ui_data.text_data.font_size		= { GetDrawStringWidthToHandle(mission_ui_data.text_data.text.c_str(), -1, mission_ui_data.text_data.font_handle), GetFontSizeToHandle(mission_ui_data.text_data.font_handle) };

		m_screen = std::make_shared<ScreenCreator>(mission_ui_data.screen_size, Vector2D<int>(static_cast<int>(mission_ui_data.screen_size.x * 0.5f), static_cast<int>(mission_ui_data.screen_size.y * mission_ui_data.height_ratio)));
		m_text_center_pos = m_screen->GetHalfScreenSize();
	}
}

MissionUI::~MissionUI()
{

}

void MissionUI::Init()
{
	m_alpha_blend_num	= 0;
	m_is_wait			= true;
}

void MissionUI::LateUpdate()
{
	if (!m_is_active) { return; }

	CalcAlphaBlendNum();
	CalcWaitTime();
	CreateScreen();
}

void MissionUI::Draw() const
{
	if (!m_is_active) { return; }

	m_screen->Draw();
}

void MissionUI::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_alpha_blend_num < UCHAR_MAX && m_is_wait)
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(mission_ui_data.fade_in_speed * delta_time), UCHAR_MAX, false);
	}
	else if (!m_is_wait)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(mission_ui_data.fade_out_speed * delta_time), 0);
		m_is_active = m_alpha_blend_num <= 0;
	}
}

void MissionUI::CalcWaitTime()
{
	if (m_alpha_blend_num < UCHAR_MAX) { return; }

	m_wait_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_wait_timer > mission_ui_data.draw_wait_time)
	{
		m_is_wait = false;
	}
}

void MissionUI::CreateScreen()
{
	m_screen->UseScreen();

	DrawStringToHandle(
		static_cast<int>((m_screen->GetScreenSize().x - mission_ui_data.text_data.font_size.x) * 0.5f),
		static_cast<int>((m_screen->GetScreenSize().y - mission_ui_data.text_data.font_size.y) * 0.5f),
		mission_ui_data.text_data.text.c_str(), 0xffffff, mission_ui_data.text_data.font_handle);

	m_screen->UnuseScreen();
}
