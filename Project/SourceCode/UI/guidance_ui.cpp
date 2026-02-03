#include "guidance_ui.hpp"
#include "../Font/text.hpp"

GuidanceUI::GuidanceUI(const std::string& json_name) :
	data					(GuidanceUIData()),
	m_prev_device_kind		(DeviceKind::kKeyboard),
	m_current_device_kind	(DeviceKind::kKeyboard),
	m_result_screen			(nullptr),
	m_mask_screen			(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_mask_creator			(std::make_shared<MaskCreator>()),
	m_basic_shape_graphic	(nullptr),
	m_button_graphic_getter	(std::make_shared<ButtonGraphicGetter>()),
	m_alpha_blend_num		(0),
	m_blend_wait_timer		(0.0f),
	m_draw_timer			(0.0f),
	m_is_active				(false),
	m_is_wait_blend			(true),
	m_is_fade_out			(false)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/guidance_data.json", j_data))
	{
		data = j_data.at("guidance_data").at(json_name.c_str()).get<GuidanceUIData>();
	}

	// テキストを生成
	for (auto& single_button_prompt : data.single_button_prompt_data)
	{
		text::CreateText(single_button_prompt.text_data);
	}

	m_result_screen			= std::make_shared<ScreenCreator>(data.screen_size, Vector2D<int>(Window::kScreenSize.x * data.width_ratio, Window::kScreenSize.y * data.height_ratio));
	m_basic_shape_graphic	= std::make_shared<Graphicer>(data.basic_graphic_path);
	m_basic_shape_graphic->SetCenterPos(m_result_screen->GetGraphicer()->GetCenterPos());

	m_mask_screen->UseScreen();
	m_basic_shape_graphic->Draw();
	m_mask_screen->UnuseScreen();

	UpdateInputCode();
	UpdateGraphics();
	CreateResultScreen();
}

GuidanceUI::~GuidanceUI()
{

}

void GuidanceUI::Init()
{
	m_alpha_blend_num	= 0;
	m_blend_wait_timer	= 0.0f;
	m_draw_timer		= 0.0f;
	m_is_wait_blend		= true;
	m_is_fade_out		= false;
}

void GuidanceUI::LateUpdate()
{
	if (!m_is_active) { return; }

	UpdateDeviceKind();
	UpdateInputCode();
	CalcBlendWaitTime();
	CalcDrawTime();
	CalcAlphaBlendNum();

	// 描画内容に変化があった場合のみスクリーン内容を更新
	if (CanUpdateRresultScreen())
	{
		UpdateGraphics();
		CreateResultScreen();
	}
}

void GuidanceUI::Draw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }
	if (m_is_wait_blend) { return; }

	m_mask_creator->CreateMask();
	m_mask_creator->UseMask(m_mask_screen->GetScreenHandle(), true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha_blend_num);
	DrawGraph(0, 0, main_screen_handle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha_blend_num * data.basic_shape_alpha_blend_scale);
	DrawBox(0, 0, m_mask_screen->GetScreenSize().x, m_mask_screen->GetScreenSize().y, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_mask_creator->UnuseMask();
	m_mask_creator->DeleteMask();

	m_result_screen->Draw();
}

void GuidanceUI::CalcAlphaBlendNum()
{
	if (m_is_wait_blend) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (!m_is_fade_out && m_alpha_blend_num < UCHAR_MAX)
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(data.fade_in_speed * delta_time), UCHAR_MAX, false);
	}
	else if (m_is_fade_out)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(data.fade_out_speed * delta_time), 0);
		m_is_active = m_alpha_blend_num > 0;
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void GuidanceUI::CalcBlendWaitTime()
{
	if (!m_is_wait_blend) { return; }

	m_blend_wait_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_blend_wait_timer > data.blend_wait_time)
	{
		m_is_wait_blend = false;
	}
}

void GuidanceUI::CalcDrawTime()
{
	// ブレンド率が最大になってから消え始めるまでの時間を計測
	if (m_alpha_blend_num < UCHAR_MAX) { return; }

	m_draw_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_draw_timer > data.draw_time)
	{
		m_is_fade_out = true;
	}
}

void GuidanceUI::CreateResultScreen()
{
	m_result_screen->UseScreen();

	auto offset = 0;
	auto count  = 0;
	for (const auto& text : data.single_button_prompt_data)
	{
		m_input_graphic.at(count)->SetCenterPos({ offset + m_input_graphic.at(count)->GetHalfSize().x, m_result_screen->GetHalfScreenSize().y });
		m_input_graphic.at(count)->Draw();

		// 現在のテキストを左に描画される画像サイズ分ずらす
		offset += m_input_graphic.at(count)->GetSize().x;

		DrawStringToHandle(
			static_cast<int>(offset),
			static_cast<int>((m_result_screen->GetScreenSize().y - text.text_data.size.y) * 0.5f),
			text.text_data.text.c_str(),
			text.text_data.u_int_color,
			text.text_data.font_handle);

		// 次の画像・テキストは現在のテキスト + オフセット分ずらす
		offset += text.text_data.size.x + data.offset;

		++count;
	}

	m_result_screen->UnuseScreen();
}

void GuidanceUI::UpdateDeviceKind()
{
	m_prev_device_kind		= m_current_device_kind;
	m_current_device_kind	= InputChecker::GetInstance()->GetCurrentInputDevice();
}

void GuidanceUI::UpdateInputCode()
{
	m_prev_input_code = m_current_input_code;
	m_current_input_code.clear();

	// 入力デバイスおよびキー割り当てに対応した入力コードを取得
	const auto command = CommandHandler::GetInstance();
	for (const auto& single_button_prompt : data.single_button_prompt_data)
	{
		const auto input_code = command->GetInputCode(
			m_current_device_kind,
			single_button_prompt.command_kind,
			single_button_prompt.command_slot_kind);

		m_current_input_code.emplace_back(input_code);
	}
}

void GuidanceUI::UpdateGraphics()
{
	// 画像を設定
	m_input_graphic.clear();
	for (const auto& code : m_current_input_code)
	{
		const auto graphic = m_button_graphic_getter->GetButtonGraphicer(code);
		graphic->SetScale(data.input_graphic_scale);
		m_input_graphic.emplace_back(graphic);
	}
}

const bool GuidanceUI::CanUpdateRresultScreen() const
{
	const auto is_change_device = m_prev_device_kind != m_current_device_kind;	// 入力デバイスに変更があった
	const auto is_change_code	= m_prev_input_code  != m_current_input_code;	// 入力コードに変更があった

	return is_change_device || is_change_code;
}
