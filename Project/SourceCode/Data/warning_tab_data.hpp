#pragma once
#include "../Data/text_data.hpp"

struct WarningTabData
{
	int				priority					= 0;
	TextData		warning_text_data			= {};
	TextData		decide_text_data			= {};
	TextData		back_text_data				= {};
	Vector2D<int>	first_button_center_offset	= {};
	Vector2D<int>   warning_icon_offset			= {};
	float			warning_icon_scale			= 1.0f;
	int				button_pos_interval			= 0;
	float			fade_speed					= 0.0f;
	int				alpha_blend_num				= 0;
	std::string		restart_text				= "";
	std::string		quit_game_text				= "";
	std::string		exit_text					= "";
	Vector2D<int>	button_prompt_screen_size	= {};
	float			height_ratio				= 0.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, WarningTabData& warning_tab_data)
{
	j_data.at("priority")					.get_to(warning_tab_data.priority);
	j_data.at("warning_text_data")			.get_to(warning_tab_data.warning_text_data);
	j_data.at("decide_text_data")			.get_to(warning_tab_data.decide_text_data);
	j_data.at("back_text_data")				.get_to(warning_tab_data.back_text_data);
	j_data.at("first_button_center_offset")	.get_to(warning_tab_data.first_button_center_offset);
	j_data.at("warning_icon_offset")		.get_to(warning_tab_data.warning_icon_offset);
	j_data.at("warning_icon_scale")			.get_to(warning_tab_data.warning_icon_scale);
	j_data.at("button_pos_interval")		.get_to(warning_tab_data.button_pos_interval);
	j_data.at("fade_speed")					.get_to(warning_tab_data.fade_speed);
	j_data.at("alpha_blend_num")			.get_to(warning_tab_data.alpha_blend_num);
	j_data.at("restart_text")				.get_to(warning_tab_data.restart_text);
	j_data.at("quit_game_text")				.get_to(warning_tab_data.quit_game_text);
	j_data.at("exit_text")					.get_to(warning_tab_data.exit_text);
	j_data.at("button_prompt_screen_size")	.get_to(warning_tab_data.button_prompt_screen_size);
	j_data.at("height_ratio")				.get_to(warning_tab_data.height_ratio);
}

inline void to_json(nlohmann::json& j_data, const WarningTabData& warning_tab_data)
{
	j_data = nlohmann::json
	{
		{ "priority",						warning_tab_data.priority },
		{ "warning_text_data",				warning_tab_data.warning_text_data },
		{ "decide_text_data",				warning_tab_data.decide_text_data },
		{ "back_text_data",					warning_tab_data.back_text_data },
		{ "first_button_center_offset",		warning_tab_data.first_button_center_offset },
		{ "warning_icon_offset",			warning_tab_data.warning_icon_offset },
		{ "warning_icon_scale",				warning_tab_data.warning_icon_scale },
		{ "button_pos_interval",			warning_tab_data.button_pos_interval },
		{ "fade_speed",						warning_tab_data.fade_speed },
		{ "alpha_blend_num",				warning_tab_data.alpha_blend_num },
		{ "restart_text",					warning_tab_data.restart_text },
		{ "quit_game_text",					warning_tab_data.quit_game_text },
		{ "exit_text",						warning_tab_data.exit_text },
		{ "button_prompt_screen_size",		warning_tab_data.button_prompt_screen_size },
		{ "height_ratio",					warning_tab_data.height_ratio },
	};
}
#pragma endregion
