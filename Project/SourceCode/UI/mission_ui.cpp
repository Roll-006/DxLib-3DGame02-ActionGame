#include "mission_ui.hpp"
#include "../Font/text.hpp"

MissionUI::MissionUI(const std::string& json_name) :
	data				(MissionUIData()),
	m_result_screen		(nullptr),
	m_alpha_blend_num	(0),
	m_blend_wait_timer	(0.0f),
	m_draw_timer		(0.0f),
	m_is_active			(false),
	m_is_wait_blend		(true),
	m_is_fade_out		(false)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/mission_data.json", j_data))
	{
		data = j_data.at("mission_data").at(json_name.c_str()).get<MissionUIData>();

		text::CreateText(data.text_data);

		m_result_screen	= std::make_shared<ScreenCreator>(data.screen_size, Vector2D<int>(Window::kCenterPos.x, Window::kScreenSize.y * data.height_ratio));
	}
}

MissionUI::~MissionUI()
{

}

void MissionUI::Init()
{
	m_alpha_blend_num	= 0;
	m_blend_wait_timer	= 0.0f;
	m_draw_timer		= 0.0f;
	m_is_wait_blend		= true;
	m_is_fade_out		= false;
}

void MissionUI::LateUpdate()
{
	if (!m_is_active) { return; }

	CalcBlendWaitTime();
	CalcDrawTime();
	CalcAlphaBlendNum();
	CreateResultScreen();
}

void MissionUI::Draw() const
{
	if (!m_is_active) { return; }

	m_result_screen->Draw();
}

void MissionUI::CalcAlphaBlendNum()
{
	if (m_is_wait_blend) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (!m_is_fade_out && m_alpha_blend_num < UCHAR_MAX)
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(data.fade_in_speed  * delta_time), UCHAR_MAX, false);
	}
	else if (m_is_fade_out)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(data.fade_out_speed * delta_time), 0);
		m_is_active = m_alpha_blend_num > 0;
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void MissionUI::CalcBlendWaitTime()
{
	if (!m_is_wait_blend) { return; }

	m_blend_wait_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_blend_wait_timer > data.blend_wait_time)
	{
		m_is_wait_blend = false;
	}
}

void MissionUI::CalcDrawTime()
{
	// ブレンド率が最大になってから消え始めるまでの時間を計測
	if (m_alpha_blend_num < UCHAR_MAX) { return; }

	m_draw_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_draw_timer > data.draw_time)
	{
		m_is_fade_out = true;
	}
}

void MissionUI::CreateResultScreen()
{
	if (m_is_wait_blend) { return; }

	m_result_screen->UseScreen();

	DrawStringToHandle(
		static_cast<int>((m_result_screen->GetScreenSize().x - data.text_data.size.x) * 0.5f),
		static_cast<int>((m_result_screen->GetScreenSize().y - data.text_data.size.y) * 0.5f),
		data.text_data.text.c_str(), data.text_data.u_int_color, data.text_data.font_handle);

	m_result_screen->UnuseScreen();
}
