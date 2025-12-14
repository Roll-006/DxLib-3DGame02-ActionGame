#include "button_prompt.hpp"

ButtonPrompt::ButtonPrompt(const std::string& prompt_name) :
	m_prev_device_kind		(DeviceKind::kKeyboard),
	m_current_device_kind	(DeviceKind::kKeyboard),
	m_result_screen			(nullptr),
	m_prev_button_index		(-1),
	m_button_graphic_getter	(std::make_shared<ButtonGraphicGetter>())
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/button_prompt_data.json", j_data))
	{
		data = j_data.at("button_prompt_data").at(prompt_name).get<ButtonPromptData>();

		// テキストを生成
		const auto font_handler = FontHandler::GetInstance();
		for (auto& single_button_prompt : data.single_button_prompt_data)
		{
			single_button_prompt.text_data.text			= type_converter::ConvertUTF8ToShiftJIS(single_button_prompt.text_data.text);
			single_button_prompt.text_data.font_handle	= font_handler->GetFontHandle(single_button_prompt.text_data.font_path);
			single_button_prompt.text_data.u_int_color	= type_converter::ConvertHEXToUINT(single_button_prompt.text_data.hex_color);
			single_button_prompt.text_data.font_size	= { GetDrawStringWidthToHandle(single_button_prompt.text_data.text.c_str(), -1, single_button_prompt.text_data.font_handle), GetFontSizeToHandle(single_button_prompt.text_data.font_handle) };
		}

		m_result_screen = std::make_shared<ScreenCreator>(data.screen_size, Vector2D<int>(Window::kCenterPos.x, static_cast<int>(Window::kScreenSize.y * data.height_ratio)));

		data.text_data.font_handle	= font_handler->GetFontHandle(data.text_data.font_path);
		data.text_data.u_int_color	= type_converter::ConvertHEXToUINT(data.text_data.hex_color);		
	}

	UpdateInputCode();
	UpdateGraphics();
	CalcLeftmostPos();
	CreateResultScreen();
}

ButtonPrompt::~ButtonPrompt()
{

}

void ButtonPrompt::Update(const int current_button_index)
{
	UpdateDeviceKind();
	UpdateInputCode();
	UpdateUIButtonExplanatoryText(current_button_index);

	// 描画内容に変化があった場合のみスクリーン内容を更新
	if (CanUpdateRresultScreen())
	{
		UpdateGraphics();
		CalcLeftmostPos();
		CreateResultScreen();
	}
}

void ButtonPrompt::Draw() const
{
	m_result_screen->Draw();

	DrawStringToHandle(
		static_cast<int>(m_result_screen->GetGraphicer()->GetCenterPos().x - data.text_data.font_size.x * 0.5f + data.text_data.offset.x),
		static_cast<int>(m_result_screen->GetGraphicer()->GetCenterPos().y - data.text_data.font_size.y * 0.5f + data.text_data.offset.y),
		data.text_data.text.c_str(),
		data.text_data.u_int_color,
		data.text_data.font_handle);
}

void ButtonPrompt::AddExplanatoryText(const int button_index, const std::string& explanatory)
{
	m_ui_button_explanatory_texts[button_index] = explanatory;
}

bool ButtonPrompt::CanUpdateRresultScreen() const
{
	const auto is_change_device = m_prev_device_kind != m_current_device_kind;	// 入力デバイスに変更があった
	const auto is_change_code	= m_prev_input_code  != m_current_input_code;	// 入力コードに変更があった

	return is_change_device || is_change_code;
}

void ButtonPrompt::CreateResultScreen()
{
	m_result_screen->UseScreen();

	auto offset = m_leftmost_pos;
	auto count  = 0;
	for (const auto& text : data.single_button_prompt_data)
	{
		m_button_graphic.at(count)->SetCenterPos({ offset + m_button_graphic.at(count)->GetHalfSize().x, m_result_screen->GetHalfScreenSize().y });
		m_button_graphic.at(count)->Draw();

		// 現在のテキストを左に描画される画像サイズ分ずらす
		offset += m_button_graphic.at(count)->GetSize().x;
		
		DrawStringToHandle(
			static_cast<int>(offset),
			static_cast<int>((m_result_screen->GetScreenSize().y - text.text_data.font_size.y) * 0.5f),
			text.text_data.text.c_str(),
			text.text_data.u_int_color,
			text.text_data.font_handle);

		// 次の画像・テキストは現在のテキスト + オフセット分ずらす
		offset += text.text_data.font_size.x + data.offset;

		++count;
	}

	m_result_screen->UnuseScreen();
}

void ButtonPrompt::UpdateDeviceKind()
{
	m_prev_device_kind		= m_current_device_kind;
	m_current_device_kind	= InputChecker::GetInstance()->GetCurrentInputDevice();
}

void ButtonPrompt::UpdateInputCode()
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

void ButtonPrompt::UpdateUIButtonExplanatoryText(const int current_button_index)
{
	// 現在選択されているボタンUIに対応する説明文を取得し設定
	if (current_button_index != m_prev_button_index)
	{
		data.text_data.text			= m_ui_button_explanatory_texts.contains(current_button_index) ? m_ui_button_explanatory_texts.at(current_button_index) : "";
		data.text_data.font_size	= { GetDrawStringWidthToHandle(data.text_data.text.c_str(), -1, data.text_data.font_handle), GetFontSizeToHandle(data.text_data.font_handle) };
	}

	m_prev_button_index = current_button_index;
}

void ButtonPrompt::UpdateGraphics()
{
	// 画像を設定
	m_button_graphic.clear();
	for (const auto& code : m_current_input_code)
	{
		const auto graphic = m_button_graphic_getter->GetButtonGraphicer(code);
		graphic->SetScale(data.graphic_scale);
		m_button_graphic.emplace_back(graphic);
	}
}

void ButtonPrompt::CalcLeftmostPos()
{
	auto count			= 0;
	auto total_width	= 0;

	// テキストの数分だけ画像とテキストの幅を加算
	// 最初の画像以外はオフセット幅を同時に加算
	for (const auto& text : data.single_button_prompt_data)
	{
		const auto text_width = text.text_data.font_size.x;
		total_width += count != 0 ? data.offset + text_width : text_width;
		total_width += m_button_graphic.at(count)->GetSize().x;

		++count;
	}

	m_leftmost_pos = (m_result_screen->GetScreenSize().x - total_width) * 0.5f;
}
