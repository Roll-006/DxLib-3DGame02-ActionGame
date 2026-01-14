#pragma once
#include "../Data/text_data.hpp"

struct TitleTabData
{
	int						priority					= 0;
	TextData				warning_text_data			= {};
	std::vector<TextData>	explanatory_text_data		= {};
	std::vector<TextData>	text_data					= {};
	Vector2D<int>			first_button_center_offset	= {};
	int						button_pos_interval			= 0;
	float					fade_speed					= 0.0f;
	std::string				button_prompt_name			= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, TitleTabData& title_tab_data)
{
	j_data.at("priority")					.get_to(title_tab_data.priority);
	j_data.at("warning_text_data")			.get_to(title_tab_data.warning_text_data);
	j_data.at("explanatory_text_data")		.get_to(title_tab_data.explanatory_text_data);
	j_data.at("text_data")					.get_to(title_tab_data.text_data);
	j_data.at("first_button_center_offset")	.get_to(title_tab_data.first_button_center_offset);
	j_data.at("button_pos_interval")		.get_to(title_tab_data.button_pos_interval);
	j_data.at("fade_speed")					.get_to(title_tab_data.fade_speed);
	j_data.at("button_prompt_name")			.get_to(title_tab_data.button_prompt_name);
}

inline void to_json(nlohmann::json& j_data, const TitleTabData& title_tab_data)
{
	j_data = nlohmann::json
	{
		{ "priority",					title_tab_data.priority },
		{ "warning_text_data",			title_tab_data.warning_text_data },
		{ "explanatory_text_data",		title_tab_data.explanatory_text_data },
		{ "text_data",					title_tab_data.text_data },
		{ "first_button_center_offset",	title_tab_data.first_button_center_offset },
		{ "button_pos_interval",		title_tab_data.button_pos_interval },
		{ "fade_speed",					title_tab_data.fade_speed },
		{ "button_prompt_name",			title_tab_data.button_prompt_name },
	};
}
#pragma endregion
