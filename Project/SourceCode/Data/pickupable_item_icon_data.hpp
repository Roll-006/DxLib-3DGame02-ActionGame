#pragma once
#include "../Calculation/vector_3d.hpp"
#include "../Calculation/vector_2d.hpp"
#include "../Data/text_data.hpp"

struct PickupableItemIconData final
{
	VECTOR			icon_offset									= {};
	float			icon_size									= 0.0f;
	Vector2D<int>	cursor_offset								= {};
	int				offset										= 0;
	Vector2D<int>	screen_size									= {};
	TextData		text_data									= {};
	Vector2D<float> mask_graphic_scale							= {};
	Vector2D<float> explanatory_text_box_blur_graphic_scale		= {};
	Vector2D<int>	mask_graphic_offset							= {};
	Vector2D<int>	explanatory_text_box_blur_graphic_offset	= {};
	float			icon_size_ratio								= 1.0f;
	float			pickupable_cursor_graphic_scale				= 0.0f;
	float			candidate_cursor_graphic_scale				= 0.0f;
	float			button_icon_graphic_scale					= 0.0f;
	std::string		pickupable_cursor_graphic_path				= "";
	std::string		candidate_cursor_graphic_path				= "";
	std::string		explanatory_text_box_blur_graphic_path		= "";
	std::string		mask_graphic_path							= "";
	std::string		assault_rifle_ammo_box_name					= "";
	std::string		rocket_bomb_box_name						= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, PickupableItemIconData& pickupable_item_icon_data)
{
	j_data.at("icon_offset")								.get_to(pickupable_item_icon_data.icon_offset);
	j_data.at("icon_size")									.get_to(pickupable_item_icon_data.icon_size);
	j_data.at("cursor_offset")								.get_to(pickupable_item_icon_data.cursor_offset);
	j_data.at("offset")										.get_to(pickupable_item_icon_data.offset);
	j_data.at("screen_size")								.get_to(pickupable_item_icon_data.screen_size);
	j_data.at("text_data")									.get_to(pickupable_item_icon_data.text_data);
	j_data.at("mask_graphic_scale")							.get_to(pickupable_item_icon_data.mask_graphic_scale);
	j_data.at("explanatory_text_box_blur_graphic_scale")	.get_to(pickupable_item_icon_data.explanatory_text_box_blur_graphic_scale);
	j_data.at("mask_graphic_offset")						.get_to(pickupable_item_icon_data.mask_graphic_offset);
	j_data.at("explanatory_text_box_blur_graphic_offset")	.get_to(pickupable_item_icon_data.explanatory_text_box_blur_graphic_offset);
	j_data.at("icon_size_ratio")							.get_to(pickupable_item_icon_data.icon_size_ratio);
	j_data.at("pickupable_cursor_graphic_scale")			.get_to(pickupable_item_icon_data.pickupable_cursor_graphic_scale);
	j_data.at("candidate_cursor_graphic_scale")				.get_to(pickupable_item_icon_data.candidate_cursor_graphic_scale);
	j_data.at("button_icon_graphic_scale")					.get_to(pickupable_item_icon_data.button_icon_graphic_scale);
	j_data.at("pickupable_cursor_graphic_path")				.get_to(pickupable_item_icon_data.pickupable_cursor_graphic_path);
	j_data.at("candidate_cursor_graphic_path")				.get_to(pickupable_item_icon_data.candidate_cursor_graphic_path);
	j_data.at("explanatory_text_box_blur_graphic_path")		.get_to(pickupable_item_icon_data.explanatory_text_box_blur_graphic_path);
	j_data.at("mask_graphic_path")							.get_to(pickupable_item_icon_data.mask_graphic_path);
	j_data.at("assault_rifle_ammo_box_name")				.get_to(pickupable_item_icon_data.assault_rifle_ammo_box_name);
	j_data.at("rocket_bomb_box_name")						.get_to(pickupable_item_icon_data.rocket_bomb_box_name);
}

inline void to_json(nlohmann::json& j_data, const PickupableItemIconData& pickupable_item_icon_data)
{
	j_data = nlohmann::json
	{
		{ "icon_offset",								pickupable_item_icon_data.icon_offset },
		{ "icon_size",									pickupable_item_icon_data.icon_size },
		{ "cursor_offset",								pickupable_item_icon_data.cursor_offset },
		{ "offset",										pickupable_item_icon_data.offset },
		{ "screen_size",								pickupable_item_icon_data.screen_size },
		{ "text_data",									pickupable_item_icon_data.text_data },
		{ "mask_graphic_scale",							pickupable_item_icon_data.mask_graphic_scale },
		{ "explanatory_text_box_blur_graphic_scale",	pickupable_item_icon_data.explanatory_text_box_blur_graphic_scale },
		{ "mask_graphic_offset",						pickupable_item_icon_data.mask_graphic_offset },
		{ "explanatory_text_box_blur_graphic_offset",	pickupable_item_icon_data.explanatory_text_box_blur_graphic_offset },
		{ "icon_size_ratio",							pickupable_item_icon_data.icon_size_ratio },
		{ "pickupable_cursor_graphic_scale",			pickupable_item_icon_data.pickupable_cursor_graphic_scale },
		{ "candidate_cursor_graphic_scale",				pickupable_item_icon_data.candidate_cursor_graphic_scale },
		{ "button_icon_graphic_scale",					pickupable_item_icon_data.button_icon_graphic_scale },
		{ "pickupable_cursor_graphic_path",				pickupable_item_icon_data.pickupable_cursor_graphic_path },
		{ "candidate_cursor_graphic_path",				pickupable_item_icon_data.candidate_cursor_graphic_path },
		{ "explanatory_text_box_blur_graphic_path",		pickupable_item_icon_data.explanatory_text_box_blur_graphic_path },
		{ "mask_graphic_path",							pickupable_item_icon_data.mask_graphic_path },
		{ "assault_rifle_ammo_box_name",				pickupable_item_icon_data.assault_rifle_ammo_box_name },
		{ "rocket_bomb_box_name",						pickupable_item_icon_data.rocket_bomb_box_name },
	};
}
#pragma endregion
