#pragma once
#include "../JSON/json_loader.hpp"
#include "../Data/purpose_ui_data.hpp"
#include "../GameTime/game_time_manager.hpp"
#include "../Calculation/math.hpp"

class PurposeUI final
{
public:
	PurposeUI(const std::string& json_name);
	~PurposeUI();

	void Init();
	void LateUpdate();
	void Draw(const int main_screen_handle) const;

	void Activate() { m_is_active = true; }

private:
	void CalcAlphaBlendNum();
	void CalcBlendWaitTime();
	void CalcDrawTime();
	void CreateResultScreen();

private:
	PurposeUIData					data;

	std::shared_ptr<ScreenCreator>	m_result_screen;
	std::shared_ptr<ScreenCreator>	m_mask_screen;
	std::shared_ptr<MaskCreator>	m_mask_creator;
	std::shared_ptr<Graphicer>		m_basic_shape_graphic;
	int								m_alpha_blend_num;
	float							m_blend_wait_timer;
	float							m_draw_timer;
	bool							m_is_active;
	bool							m_is_wait_blend;
	bool							m_is_fade_out;

	friend void from_json(const nlohmann::json& j_data, PurposeUI& purpose_ui);
	friend void to_json  (nlohmann::json& j_data, const PurposeUI& purpose_ui);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, PurposeUI& purpose_ui)
{
	j_data.at("purpose_ui_data").get_to(purpose_ui.data);
}

inline void to_json(nlohmann::json& j_data, const PurposeUI& purpose_ui)
{
	j_data = nlohmann::json
	{
		{ "purpose_ui_data", purpose_ui.data },
	};
}
#pragma endregion
