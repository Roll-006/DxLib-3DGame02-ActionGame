#pragma once
#include <string>

struct SubMenuSelectButtonData final
{
	float		fade_in_speed				= 0.0f;
	float		fade_out_speed				= 0.0f;
	int			non_select_alpha_blend_num	= UCHAR_MAX;
	float		non_select_scale			= 1.0f;
	float		select_scale				= 1.0f;
	float		selecting_button_distance	= 0.0f;
	float		move_speed					= 0.0f;
	std::string dark_frame_graphic_path		= "";
	std::string light_frame_graphic_path	= "";
	std::string selecting_graphic_path		= "";
	std::string selecting_movie_path		= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, SubMenuSelectButtonData& sub_menu_select_button_data)
{
	j_data.at("fade_in_speed")				.get_to(sub_menu_select_button_data.fade_in_speed);
	j_data.at("fade_out_speed")				.get_to(sub_menu_select_button_data.fade_out_speed);
	j_data.at("non_select_alpha_blend_num")	.get_to(sub_menu_select_button_data.non_select_alpha_blend_num);
	j_data.at("non_select_scale")			.get_to(sub_menu_select_button_data.non_select_scale);
	j_data.at("select_scale")				.get_to(sub_menu_select_button_data.select_scale);
	j_data.at("selecting_button_distance")	.get_to(sub_menu_select_button_data.selecting_button_distance);
	j_data.at("move_speed")					.get_to(sub_menu_select_button_data.move_speed);
	j_data.at("dark_frame_graphic_path")	.get_to(sub_menu_select_button_data.dark_frame_graphic_path);
	j_data.at("light_frame_graphic_path")	.get_to(sub_menu_select_button_data.light_frame_graphic_path);
	j_data.at("selecting_graphic_path")		.get_to(sub_menu_select_button_data.selecting_graphic_path);
	j_data.at("selecting_movie_path")		.get_to(sub_menu_select_button_data.selecting_movie_path);
}

inline void to_json(nlohmann::json& j_data, const SubMenuSelectButtonData& sub_menu_select_button_data)
{
	j_data = nlohmann::json
	{
		{ "fade_in_speed",				sub_menu_select_button_data.fade_in_speed },
		{ "fade_out_speed",				sub_menu_select_button_data.fade_out_speed },
		{ "non_select_alpha_blend_num",	sub_menu_select_button_data.non_select_alpha_blend_num },
		{ "non_select_scale",			sub_menu_select_button_data.non_select_scale },
		{ "select_scale",				sub_menu_select_button_data.select_scale },
		{ "selecting_button_distance",	sub_menu_select_button_data.selecting_button_distance },
		{ "move_speed",					sub_menu_select_button_data.move_speed },
		{ "dark_frame_graphic_path",	sub_menu_select_button_data.dark_frame_graphic_path },
		{ "light_frame_graphic_path",	sub_menu_select_button_data.light_frame_graphic_path },
		{ "selecting_graphic_path",		sub_menu_select_button_data.selecting_graphic_path },
		{ "selecting_movie_path",		sub_menu_select_button_data.selecting_movie_path },
	};
}
#pragma endregion
