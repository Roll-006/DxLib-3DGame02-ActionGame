#pragma once
#include "../Calculation/vector_3d.hpp"
#include "../Calculation/vector_2d.hpp"
#include "../Data/text_data.hpp"

struct MeleeTargetIconData final
{
	VECTOR			icon_offset									= {};
	float			icon_size									= 0.0f;
	Vector2D<int>	screen_size									= {};
	Vector2D<int>	cursor_offset								= {};
	TextData		text_data									= {};
	float			melee_cursor_graphic_scale					= 0.0f;
	float			down_cursor_graphic_scale					= 0.0f;
	float			mask_graphic_scale							= 0.0f;
	float			button_icon_graphic_scale					= 0.0f;
	float			explanatory_text_box_blur_graphic_scale		= 0.0f;
	Vector2D<int>	mask_graphic_offset							= {};
	Vector2D<int>	explanatory_text_box_blur_graphic_offset	= {};
	float			icon_size_ratio								= 1.0f;
	std::string		melee_cursor_graphic_path					= "";
	std::string		down_cursor_graphic_path					= "";
	std::string		explanatory_text_box_blur_graphic_path		= "";
	std::string		mask_graphic_path							= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, MeleeTargetIconData& melee_target_icon_data)
{
	j_data.at("icon_offset")								.get_to(melee_target_icon_data.icon_offset);
	j_data.at("icon_size")									.get_to(melee_target_icon_data.icon_size);
	j_data.at("screen_size")								.get_to(melee_target_icon_data.screen_size);
	j_data.at("cursor_offset")								.get_to(melee_target_icon_data.cursor_offset);
	j_data.at("text_data")									.get_to(melee_target_icon_data.text_data);
	j_data.at("melee_cursor_graphic_scale")					.get_to(melee_target_icon_data.melee_cursor_graphic_scale);
	j_data.at("down_cursor_graphic_scale")					.get_to(melee_target_icon_data.down_cursor_graphic_scale);
	j_data.at("mask_graphic_scale")							.get_to(melee_target_icon_data.mask_graphic_scale);
	j_data.at("button_icon_graphic_scale")					.get_to(melee_target_icon_data.button_icon_graphic_scale);
	j_data.at("explanatory_text_box_blur_graphic_scale")	.get_to(melee_target_icon_data.explanatory_text_box_blur_graphic_scale);
	j_data.at("mask_graphic_offset")						.get_to(melee_target_icon_data.mask_graphic_offset);
	j_data.at("explanatory_text_box_blur_graphic_offset")	.get_to(melee_target_icon_data.explanatory_text_box_blur_graphic_offset);
	j_data.at("icon_size_ratio")							.get_to(melee_target_icon_data.icon_size_ratio);
	j_data.at("melee_cursor_graphic_path")					.get_to(melee_target_icon_data.melee_cursor_graphic_path);
	j_data.at("down_cursor_graphic_path")					.get_to(melee_target_icon_data.down_cursor_graphic_path);
	j_data.at("explanatory_text_box_blur_graphic_path")		.get_to(melee_target_icon_data.explanatory_text_box_blur_graphic_path);
	j_data.at("mask_graphic_path")							.get_to(melee_target_icon_data.mask_graphic_path);
}

inline void to_json(nlohmann::json& j_data, const MeleeTargetIconData& melee_target_icon_data)
{
	j_data = nlohmann::json
	{
		{ "icon_offset",								melee_target_icon_data.icon_offset },
		{ "icon_size",									melee_target_icon_data.icon_size },
		{ "screen_size",								melee_target_icon_data.screen_size },
		{ "cursor_offset",								melee_target_icon_data.cursor_offset },
		{ "text_data",									melee_target_icon_data.text_data },
		{ "melee_cursor_graphic_scale",					melee_target_icon_data.melee_cursor_graphic_scale },
		{ "down_cursor_graphic_scale",					melee_target_icon_data.down_cursor_graphic_scale },
		{ "mask_graphic_scale",							melee_target_icon_data.mask_graphic_scale },
		{ "button_icon_graphic_scale",					melee_target_icon_data.button_icon_graphic_scale },
		{ "explanatory_text_box_blur_graphic_scale",	melee_target_icon_data.explanatory_text_box_blur_graphic_scale },
		{ "mask_graphic_offset",						melee_target_icon_data.mask_graphic_offset },
		{ "explanatory_text_box_blur_graphic_offset",	melee_target_icon_data.explanatory_text_box_blur_graphic_offset },
		{ "icon_size_ratio",							melee_target_icon_data.icon_size_ratio },
		{ "melee_cursor_graphic_path",					melee_target_icon_data.melee_cursor_graphic_path },
		{ "down_cursor_graphic_path",					melee_target_icon_data.down_cursor_graphic_path },
		{ "explanatory_text_box_blur_graphic_path",		melee_target_icon_data.explanatory_text_box_blur_graphic_path },
		{ "mask_graphic_path",							melee_target_icon_data.mask_graphic_path },
	};
}
#pragma endregion
