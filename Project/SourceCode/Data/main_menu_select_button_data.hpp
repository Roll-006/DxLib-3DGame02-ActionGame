#pragma once

struct MainMenuSelectButtonData final
{
	float	fade_in_speed				= 0.0f;
	float	fade_out_speed				= 0.0f;
	int		screen_width				= 0;
	int		screen_height_offset		= 0;
	int		non_select_alpha_blend_num	= UCHAR_MAX;
	float	non_select_scale			= 0.0f;
	float	select_scale				= 0.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, MainMenuSelectButtonData& main_menu_select_button_data)
{
	j_data.at("fade_in_speed")				.get_to(main_menu_select_button_data.fade_in_speed);
	j_data.at("fade_out_speed")				.get_to(main_menu_select_button_data.fade_out_speed);
	j_data.at("screen_width")				.get_to(main_menu_select_button_data.screen_width);
	j_data.at("screen_height_offset")		.get_to(main_menu_select_button_data.screen_height_offset);
	j_data.at("non_select_alpha_blend_num")	.get_to(main_menu_select_button_data.non_select_alpha_blend_num);
	j_data.at("non_select_scale")			.get_to(main_menu_select_button_data.non_select_scale);
	j_data.at("select_scale")				.get_to(main_menu_select_button_data.select_scale);
}

inline void to_json(nlohmann::json& j_data, const MainMenuSelectButtonData& main_menu_select_button_data)
{
	j_data = nlohmann::json
	{
		{ "fade_in_speed",				main_menu_select_button_data.fade_in_speed },
		{ "fade_out_speed",				main_menu_select_button_data.fade_out_speed },
		{ "screen_width",				main_menu_select_button_data.screen_width },
		{ "screen_height_offset",		main_menu_select_button_data.screen_height_offset },
		{ "non_select_alpha_blend_num",	main_menu_select_button_data.non_select_alpha_blend_num },
		{ "non_select_scale",			main_menu_select_button_data.non_select_scale },
		{ "select_scale",				main_menu_select_button_data.select_scale },
	};
}
#pragma endregion
