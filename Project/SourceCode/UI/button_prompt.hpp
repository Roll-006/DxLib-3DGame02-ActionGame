#pragma once
#include "../Data/text_data.hpp"
#include "../Data/button_prompt_data.hpp"
#include "../Part/screen_creator.hpp"
#include "../Command/command_handler.hpp"
#include "../Part/button_graphic_getter.hpp"

class ButtonPrompt final
{
public:
	ButtonPrompt(const std::string& prompt_name);
	~ButtonPrompt();

	void Update(const int current_button_index);
	void Draw() const;

	/// @brief ボタンUIに対応する説明文を追加
	void AddExplanatoryText(const int button_index, const std::string& explanatory);

private:
	[[nodiscard]] bool CanUpdateRresultScreen() const;

	void CreateResultScreen();

	void UpdateDeviceKind();
	void UpdateInputCode();
	void UpdateUIButtonExplanatoryText(const int current_button_index);
	void UpdateGraphics();
	void CalcLeftmostPos();

private:
	ButtonPromptData data;

	DeviceKind								m_prev_device_kind;			// 以前の入力デバイス
	DeviceKind								m_current_device_kind;		// 現在の入力デバイス
	std::vector<InputCode>					m_prev_input_code;			// 以前の入力コード
	std::vector<InputCode>					m_current_input_code;		// 現在の入力コード

	std::vector<std::shared_ptr<Graphicer>>	m_button_graphic;
	std::shared_ptr<ScreenCreator>			m_result_screen;
	std::shared_ptr<ButtonGraphicGetter>	m_button_graphic_getter;
	int										m_leftmost_pos;

	int										m_prev_button_index;
	std::unordered_map<int, std::string>	m_ui_button_explanatory_texts;		// key : セレクトインデックス, value : 説明文
};
