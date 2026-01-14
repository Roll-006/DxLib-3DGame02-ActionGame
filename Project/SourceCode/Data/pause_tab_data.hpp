#pragma once
#include "../Data/text_data.hpp"

struct PauseTabData
{
	int						priority					= 0;
	TextData				warning_restart_text_data	= {};
	TextData				warning_quit_game_text_data = {};
	std::vector<TextData>	explanatory_text_data		= {};
	std::vector<TextData>	text_data					= {};
	Vector2D<int>			first_button_center_offset	= {};
	int						button_pos_interval			= 0;
	float					fade_speed					= 0.0f;
	int						background_alpha_blend_num	= UCHAR_MAX;
	std::string				button_prompt_name			= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, PauseTabData& pause_tab_data)
{
	j_data.at("priority")					.get_to(pause_tab_data.priority);
	j_data.at("warning_restart_text_data")	.get_to(pause_tab_data.warning_restart_text_data);
	j_data.at("warning_quit_game_text_data").get_to(pause_tab_data.warning_quit_game_text_data);
	j_data.at("explanatory_text_data")		.get_to(pause_tab_data.explanatory_text_data);
	j_data.at("text_data")					.get_to(pause_tab_data.text_data);
	j_data.at("first_button_center_offset")	.get_to(pause_tab_data.first_button_center_offset);
	j_data.at("button_pos_interval")		.get_to(pause_tab_data.button_pos_interval);
	j_data.at("fade_speed")					.get_to(pause_tab_data.fade_speed);
	j_data.at("background_alpha_blend_num")	.get_to(pause_tab_data.background_alpha_blend_num);
	j_data.at("button_prompt_name")			.get_to(pause_tab_data.button_prompt_name);
}

inline void to_json(nlohmann::json& j_data, const PauseTabData& pause_tab_data)
{
	j_data = nlohmann::json
	{
		{ "priority",						pause_tab_data.priority },
		{ "warning_restart_text_data",		pause_tab_data.warning_restart_text_data },
		{ "warning_quit_game_text_data",	pause_tab_data.warning_quit_game_text_data },
		{ "explanatory_text_data",			pause_tab_data.explanatory_text_data },
		{ "text_data",						pause_tab_data.text_data },
		{ "first_button_center_offset",		pause_tab_data.first_button_center_offset },
		{ "button_pos_interval",			pause_tab_data.button_pos_interval },
		{ "fade_speed",						pause_tab_data.fade_speed },
		{ "background_alpha_blend_num",		pause_tab_data.background_alpha_blend_num },
		{ "button_prompt_name",				pause_tab_data.button_prompt_name },
	};
}
#pragma endregion
