#pragma once
#include "../Font/font_handler.hpp"
#include "../Part/mask_creator.hpp"
#include "../Part/screen_creator.hpp"
#include "../Data/text_data.hpp"

struct PurposeUIData final
{
	Vector2D<int>	screen_size						= {};
	float			height_ratio					= 0.0f;
	TextData		text_data						= {};
	float			fade_in_speed					= 0.0f;
	float			fade_out_speed					= 0.0f;
	float			blend_wait_time					= 0.0f;			// アクティブ化されてから描画されるまでの待ち時間
	float			draw_time						= 0.0f;			// 描画時間
	float			basic_shape_alpha_blend_scale	= 1.0f;
	std::string		basic_graphic_path				= "";
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, PurposeUIData& purpose_ui_data)
{
	j_data.at("screen_size")					.get_to(purpose_ui_data.screen_size);
	j_data.at("height_ratio")					.get_to(purpose_ui_data.height_ratio);
	j_data.at("text_data")						.get_to(purpose_ui_data.text_data);
	j_data.at("fade_in_speed")					.get_to(purpose_ui_data.fade_in_speed);
	j_data.at("fade_out_speed")					.get_to(purpose_ui_data.fade_out_speed);
	j_data.at("blend_wait_time")				.get_to(purpose_ui_data.blend_wait_time);
	j_data.at("draw_time")						.get_to(purpose_ui_data.draw_time);
	j_data.at("basic_shape_alpha_blend_scale")	.get_to(purpose_ui_data.basic_shape_alpha_blend_scale);
	j_data.at("basic_graphic_path")				.get_to(purpose_ui_data.basic_graphic_path);
}

inline void to_json(nlohmann::json& j_data, const PurposeUIData& purpose_ui_data)
{
	j_data = nlohmann::json
	{
		{ "screen_size",					purpose_ui_data.screen_size },
		{ "height_ratio",					purpose_ui_data.height_ratio },
		{ "text_data",						purpose_ui_data.text_data },
		{ "fade_in_speed",					purpose_ui_data.fade_in_speed },
		{ "fade_out_speed",					purpose_ui_data.fade_out_speed },
		{ "blend_wait_time",				purpose_ui_data.blend_wait_time },
		{ "draw_time",						purpose_ui_data.draw_time },
		{ "basic_shape_alpha_blend_scale",	purpose_ui_data.basic_shape_alpha_blend_scale },
		{ "basic_graphic_path",				purpose_ui_data.basic_graphic_path },
	};
}
#pragma endregion
