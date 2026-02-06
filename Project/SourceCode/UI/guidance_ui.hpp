#pragma once
#include "../JSON/json_loader.hpp"
#include "../Data/guidance_ui_data.hpp"
#include "../GameTime/game_time_manager.hpp"
#include "../Calculation/math.hpp"
#include "../Command/command_handler.hpp"
#include "../Part/button_graphic_getter.hpp"

class GuidanceUI final
{
public:
	GuidanceUI(const std::string& json_name);
	~GuidanceUI();

	void Init();
	void LateUpdate();
	void Draw(const int main_screen_handle) const;

	void Activate() { m_is_active = true; }

	[[nodiscard]] const bool IsActive() const { return m_is_active; }

private:
	void CalcAlphaBlendNum();
	void CalcBlendWaitTime();
	void CalcDrawTime();
	void CreateResultScreen();

	void UpdateDeviceKind();
	void UpdateInputCode();
	void UpdateGraphics();

	/// @brief 固有の処理を持つ入力コードの更新
	/// @return true : 固有な処理が行われた, false : 行われなかった
	[[nodiscard]] const bool UniqueUpdateInputCode(std::vector<InputCode>& input_code, const SingleButtonPromptData& prompt_data, const CommandKind command_kind);

	/// @brief 固有の処理を持つ画像の更新
	/// @return true : 固有な処理が行われた, false : 行われなかった
	[[nodiscard]] const bool UniqueUpdateGraphics(const std::vector<InputCode>& input_code, std::vector<std::shared_ptr<Graphicer>>& graphicer);

	[[nodiscard]] const bool CanUpdateRresultScreen() const;

private:
	GuidanceUIData											data;

	DeviceKind												m_prev_device_kind;			// 以前の入力デバイス
	DeviceKind												m_current_device_kind;		// 現在の入力デバイス
	std::vector<std::vector<InputCode>>						m_prev_input_code;			// 以前の入力コード
	std::vector<std::vector<InputCode>>						m_current_input_code;		// 現在の入力コード

	std::shared_ptr<ScreenCreator>							m_result_screen;
	std::shared_ptr<ScreenCreator>							m_mask_screen;
	std::shared_ptr<MaskCreator>							m_mask_creator;
	std::shared_ptr<Graphicer>								m_basic_shape_graphic;
	std::vector<std::vector<std::shared_ptr<Graphicer>>>	m_input_graphic;
	std::shared_ptr<ButtonGraphicGetter>					m_button_graphic_getter;

	int														m_alpha_blend_num;
	float													m_blend_wait_timer;
	float													m_draw_timer;
	bool													m_is_active;
	bool													m_is_wait_blend;
	bool													m_is_fade_out;

	friend void from_json(const nlohmann::json& j_data, GuidanceUI& guidance_ui);
	friend void to_json(nlohmann::json& j_data, const GuidanceUI& guidance_ui);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, GuidanceUI& guidance_ui)
{
	j_data.at("guidance_ui_data").get_to(guidance_ui.data);
}

inline void to_json(nlohmann::json& j_data, const GuidanceUI& guidance_ui)
{
	j_data = nlohmann::json
	{
		{ "guidance_ui_data", guidance_ui.data },
	};
}
#pragma endregion
