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
			single_button_prompt.text_data.text = type_converter::ConvertUTF8ToShiftJIS(single_button_prompt.text_data.text);
			single_button_prompt.text_data.font_handle = font_handler->GetFontHandle(single_button_prompt.text_data.font_path);
			single_button_prompt.text_data.u_int_color = type_converter::ConvertHEXToUINT(single_button_prompt.text_data.hex_color);
			single_button_prompt.text_data.font_size = { GetDrawStringWidthToHandle(single_button_prompt.text_data.text.c_str(), -1, single_button_prompt.text_data.font_handle), GetFontSizeToHandle(single_button_prompt.text_data.font_handle) };
		}

		m_result_screen = std::make_shared<ScreenCreator>(data.screen_size, Vector2D<int>(Window::kCenterPos.x, static_cast<int>(Window::kScreenSize.y * data.height_ratio)));

		data.text_data.font_handle	= font_handler->GetFontHandle(data.text_data.font_path);
		data.text_data.u_int_color	= type_converter::ConvertHEXToUINT(data.text_data.hex_color);		
	}

	UpdateGraphics();
	CalcLeftPos();
	CreateResultScreen();
}

ButtonPrompt::~ButtonPrompt()
{

}

void ButtonPrompt::Update(const int current_button_index)
{
	m_prev_device_kind		= m_current_device_kind;
	m_current_device_kind	= InputChecker::GetInstance()->GetCurrentInputDevice();

	UpdateExplanatoryText(current_button_index);

	// 入力デバイスが変化した場合のみ描画データを変更
	if (m_prev_device_kind != m_current_device_kind)
	{
		UpdateGraphics();
		CalcLeftPos();
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
	m_explanatory_texts[button_index] = explanatory;
}

void ButtonPrompt::CreateResultScreen()
{
	m_result_screen->UseScreen();

	auto offset = m_left_pos;
	auto count  = 0;
	for (const auto& text : data.single_button_prompt_data)
	{
		m_button_graphic.at(count)->SetCenterPos({ offset + m_button_graphic.at(count)->GetHalfSize().x, m_result_screen->GetHalfScreenSize().y });
		m_button_graphic.at(count)->Draw();

		offset += m_button_graphic.at(count)->GetSize().x;
		
		DrawStringToHandle(
			static_cast<int>(offset),
			static_cast<int>((m_result_screen->GetScreenSize().y - text.text_data.font_size.y) * 0.5f),
			text.text_data.text.c_str(),
			text.text_data.u_int_color,
			text.text_data.font_handle);

		// 次のテキストは前のテキストのサイズ分ずれるため最後に加算
		offset += text.text_data.font_size.x + data.offset;

		++count;
	}

	m_result_screen->UnuseScreen();
}

void ButtonPrompt::UpdateExplanatoryText(const int current_button_index)
{
	// 現在選択されているボタンUIに対応する説明文を取得し設定
	if (current_button_index != m_prev_button_index)
	{
		data.text_data.text			= m_explanatory_texts.contains(current_button_index) ? m_explanatory_texts.at(current_button_index) : "";
		data.text_data.font_size	= { GetDrawStringWidthToHandle(data.text_data.text.c_str(), -1, data.text_data.font_handle), GetFontSizeToHandle(data.text_data.font_handle) };
	}

	m_prev_button_index = current_button_index;
}

void ButtonPrompt::UpdateGraphics()
{
	m_button_graphic.clear();

	// 入力デバイスおよびキー割り当てに対応した画像を取得
	const auto command = CommandHandler::GetInstance();
	switch (InputChecker::GetInstance()->GetCurrentInputDevice())
	{
	case DeviceKind::kKeyboard:
		for (const auto& single_button_prompt : data.single_button_prompt_data)
		{
			m_button_graphic.emplace_back(m_button_graphic_getter->GetButtonGraphicer(command->GetKeyInputCode(single_button_prompt.command_kind, single_button_prompt.command_slot_kind)));
		}
		break;

	case DeviceKind::kPad:
		for (const auto& single_button_prompt : data.single_button_prompt_data)
		{
			m_button_graphic.emplace_back(m_button_graphic_getter->GetButtonGraphicer(command->GetPadInputCode(single_button_prompt.command_kind, single_button_prompt.command_slot_kind)));
		}
		break;
	}

	// スケールを設定
	for (auto& graphic : m_button_graphic)
	{
		graphic->SetScale(data.graphic_scale);
	}
}

void ButtonPrompt::CalcLeftPos()
{
	auto count			= 0;
	auto total_width	= 0;

	// テキストの数分だけ画像とテキストの幅を加算
	// 最初の画像以外はオフセット幅を同時に加算
	for (const auto& text : data.single_button_prompt_data)
	{
		total_width += count != 0 ? data.offset + text.text_data.font_size.x : text.text_data.font_size.x;
		total_width += m_button_graphic.at(count)->GetSize().x;

		++count;
	}

	m_left_pos = (m_result_screen->GetScreenSize().x - total_width) * 0.5f;
}
