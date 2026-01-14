#pragma once
#include "../Data/text_data.hpp"

struct GameClearTabData
{
	int						priority					= 0;
	std::vector<TextData>	explanatory_text_data		= {};
	std::vector<TextData>	text_data					= {};
	Vector2D<int>			first_button_center_offset	= {};
	int						button_pos_interval			= 0;
	float					active_wait_time			= 0.0f;
	float					draw_result_wait_time		= 0.0f;
	float					scene_fade_speed			= 0.0f;
	float					screen_fade_speed			= 0.0f;
	int						background_alpha_blend_num	= UCHAR_MAX;
	std::string				button_prompt_name			= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, GameClearTabData& game_clear_tab_data)
{
	j_data.at("priority")					.get_to(game_clear_tab_data.priority);
	j_data.at("explanatory_text_data")		.get_to(game_clear_tab_data.explanatory_text_data);
	j_data.at("text_data")					.get_to(game_clear_tab_data.text_data);
	j_data.at("first_button_center_offset")	.get_to(game_clear_tab_data.first_button_center_offset);
	j_data.at("button_pos_interval")		.get_to(game_clear_tab_data.button_pos_interval);
	j_data.at("active_wait_time")			.get_to(game_clear_tab_data.active_wait_time);
	j_data.at("draw_result_wait_time")		.get_to(game_clear_tab_data.draw_result_wait_time);
	j_data.at("scene_fade_speed")			.get_to(game_clear_tab_data.scene_fade_speed);
	j_data.at("screen_fade_speed")			.get_to(game_clear_tab_data.screen_fade_speed);
	j_data.at("background_alpha_blend_num")	.get_to(game_clear_tab_data.background_alpha_blend_num);
	j_data.at("button_prompt_name")			.get_to(game_clear_tab_data.button_prompt_name);
}

inline void to_json(nlohmann::json& j_data, const GameClearTabData& game_clear_tab_data)
{
	j_data = nlohmann::json
	{
		{ "priority",					game_clear_tab_data.priority },
		{ "explanatory_text_data",		game_clear_tab_data.explanatory_text_data },
		{ "text_data",					game_clear_tab_data.text_data },
		{ "first_button_center_offset",	game_clear_tab_data.first_button_center_offset },
		{ "button_pos_interval",		game_clear_tab_data.button_pos_interval },
		{ "active_wait_time",			game_clear_tab_data.active_wait_time },
		{ "draw_result_wait_time",		game_clear_tab_data.draw_result_wait_time },
		{ "scene_fade_speed",			game_clear_tab_data.scene_fade_speed },
		{ "screen_fade_speed",			game_clear_tab_data.screen_fade_speed },
		{ "background_alpha_blend_num",	game_clear_tab_data.background_alpha_blend_num },
		{ "button_prompt_name",			game_clear_tab_data.button_prompt_name },
	};
}
#pragma endregion
