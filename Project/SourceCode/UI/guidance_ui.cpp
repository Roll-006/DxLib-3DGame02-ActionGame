//#include "guidance_ui.hpp"
//
//GuidanceUI::GuidanceUI(const std::string& json_name) :
//	data					(GuidanceUIData()),
//	m_result_screen			(nullptr),
//	m_mask_screen			(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
//	m_mask_creator			(std::make_shared<MaskCreator>()),
//	m_basic_shape_graphic	(nullptr),
//	m_alpha_blend_num		(0),
//	m_blend_wait_timer		(0.0f),
//	m_draw_timer			(0.0f),
//	m_is_active				(false),
//	m_is_wait_blend			(true),
//	m_is_fade_out			(false)
//{
//	nlohmann::json j_data;
//	if (json_loader::Load("Data/JSON/guidance_data.json", j_data))
//	{
//		data = j_data.at("guidance_data").at(json_name.c_str()).get<GuidanceUIData>();
//	}
//
//	data.text_data.font_handle	= FontHandler::GetInstance()->GetFontHandle(data.text_data.font_path);
//	data.text_data.size			= { GetDrawStringWidthToHandle(data.text_data.text.c_str(), -1, data.text_data.font_handle), GetFontSizeToHandle(data.text_data.font_handle) };
//	data.text_data.u_int_color	= type_converter::ConvertHEXToUINT(data.text_data.hex_color);
//
//	m_result_screen				= std::make_shared<ScreenCreator>(data.screen_size, Vector2D<int>(Window::kScreenSize.x * data.width_ratio, Window::kScreenSize.y * data.height_ratio));
//	m_basic_shape_graphic		= std::make_shared<Graphicer>(data.basic_graphic_path);
//	m_basic_shape_graphic->SetCenterPos(m_result_screen->GetGraphicer()->GetCenterPos());
//
//	m_mask_screen->UseScreen();
//	m_basic_shape_graphic->Draw();
//	m_mask_screen->UnuseScreen();
//}
//
//GuidanceUI::~GuidanceUI()
//{
//
//}
//
//void GuidanceUI::Init()
//{
//	m_alpha_blend_num	= 0;
//	m_blend_wait_timer	= 0.0f;
//	m_draw_timer		= 0.0f;
//	m_is_wait_blend		= true;
//	m_is_fade_out		= false;
//}
//
//void GuidanceUI::LateUpdate()
//{
//	if (!m_is_active) { return; }
//
//	CalcBlendWaitTime();
//	CalcDrawTime();
//	CalcAlphaBlendNum();
//	CreateResultScreen();
//}
//
//void GuidanceUI::Draw(const int main_screen_handle) const
//{
//	if (!m_is_active) { return; }
//	if (m_is_wait_blend) { return; }
//
//	m_mask_creator->CreateMask();
//	m_mask_creator->UseMask(m_mask_screen->GetScreenHandle(), true);
//
//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha_blend_num);
//	DrawGraph(0, 0, main_screen_handle, TRUE);
//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha_blend_num * data.basic_shape_alpha_blend_scale);
//	DrawBox(0, 0, m_mask_screen->GetScreenSize().x, m_mask_screen->GetScreenSize().y, 0x000000, TRUE);
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//
//	m_mask_creator->UnuseMask();
//	m_mask_creator->DeleteMask();
//
//	m_result_screen->Draw();
//}
//
//void GuidanceUI::CalcAlphaBlendNum()
//{
//	if (m_is_wait_blend) { return; }
//
//	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
//	if (!m_is_fade_out && m_alpha_blend_num < UCHAR_MAX)
//	{
//		math::Increase(m_alpha_blend_num, static_cast<int>(data.fade_in_speed * delta_time), UCHAR_MAX, false);
//	}
//	else if (m_is_fade_out)
//	{
//		math::Decrease(m_alpha_blend_num, static_cast<int>(data.fade_out_speed * delta_time), 0);
//		m_is_active = m_alpha_blend_num > 0;
//	}
//
//	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
//}
//
//void GuidanceUI::CalcBlendWaitTime()
//{
//	if (!m_is_wait_blend) { return; }
//
//	m_blend_wait_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
//	if (m_blend_wait_timer > data.blend_wait_time)
//	{
//		m_is_wait_blend = false;
//	}
//}
//
//void GuidanceUI::CalcDrawTime()
//{
//	// ブレンド率が最大になってから消え始めるまでの時間を計測
//	if (m_alpha_blend_num < UCHAR_MAX) { return; }
//
//	m_draw_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
//	if (m_draw_timer > data.draw_time)
//	{
//		m_is_fade_out = true;
//	}
//}
//
//void GuidanceUI::CreateResultScreen()
//{
//	m_result_screen->UseScreen();
//
//	DrawStringToHandle(
//		static_cast<int>((m_result_screen->GetScreenSize().x - data.text_data.size.x) * 0.5f),
//		static_cast<int>((m_result_screen->GetScreenSize().y - data.text_data.size.y) * 0.5f),
//		data.text_data.text.c_str(), data.text_data.u_int_color, data.text_data.font_handle);
//
//	m_result_screen->UnuseScreen();
//}
