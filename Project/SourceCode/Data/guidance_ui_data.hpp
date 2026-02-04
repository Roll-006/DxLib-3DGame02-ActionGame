#pragma once
#include "../Font/font_handler.hpp"
#include "../Part/mask_creator.hpp"
#include "../Part/screen_creator.hpp"
#include "../Data/text_data.hpp"
#include "../Data/single_button_prompt_data.hpp"

struct GuidanceUIData final
{
	Vector2D<int>						screen_size						= {};
	Vector2D<float>						screen_pos_ratio				= {};
	int									offset							= 0;
	int									leftmost_pos					= 0;
	std::vector<SingleButtonPromptData>	single_button_prompt_data		= {};
	float								input_graphic_scale				= 1.0f;
	float								fade_in_speed					= 0.0f;
	float								fade_out_speed					= 0.0f;
	float								blend_wait_time					= 0.0f;			// アクティブ化されてから描画されるまでの待ち時間
	float								draw_time						= 0.0f;			// 描画時間
	float								basic_shape_alpha_blend_scale	= 1.0f;			// 背景図形(画像)のαブレンド値の倍率
	std::string							basic_graphic_path				= "";			// 背景画像のパス
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, GuidanceUIData& guidance_ui_data)
{
	j_data.at("screen_size")					.get_to(guidance_ui_data.screen_size);
	j_data.at("screen_pos_ratio")				.get_to(guidance_ui_data.screen_pos_ratio);
	j_data.at("offset")							.get_to(guidance_ui_data.offset);
	j_data.at("leftmost_pos")					.get_to(guidance_ui_data.leftmost_pos);
	j_data.at("single_button_prompt_data")		.get_to(guidance_ui_data.single_button_prompt_data);
	j_data.at("input_graphic_scale")			.get_to(guidance_ui_data.input_graphic_scale);
	j_data.at("fade_in_speed")					.get_to(guidance_ui_data.fade_in_speed);
	j_data.at("fade_out_speed")					.get_to(guidance_ui_data.fade_out_speed);
	j_data.at("blend_wait_time")				.get_to(guidance_ui_data.blend_wait_time);
	j_data.at("draw_time")						.get_to(guidance_ui_data.draw_time);
	j_data.at("basic_shape_alpha_blend_scale")	.get_to(guidance_ui_data.basic_shape_alpha_blend_scale);
	j_data.at("basic_graphic_path")				.get_to(guidance_ui_data.basic_graphic_path);
}

inline void to_json(nlohmann::json& j_data, const GuidanceUIData& guidance_ui_data)
{
	j_data = nlohmann::json
	{
		{ "screen_size",					guidance_ui_data.screen_size },
		{ "screen_pos_ratio",				guidance_ui_data.screen_pos_ratio },
		{ "offset",							guidance_ui_data.offset },
		{ "leftmost_pos",					guidance_ui_data.leftmost_pos },
		{ "single_button_prompt_data",		guidance_ui_data.single_button_prompt_data },
		{ "input_graphic_scale",			guidance_ui_data.input_graphic_scale },
		{ "fade_in_speed",					guidance_ui_data.fade_in_speed },
		{ "fade_out_speed",					guidance_ui_data.fade_out_speed },
		{ "blend_wait_time",				guidance_ui_data.blend_wait_time },
		{ "draw_time",						guidance_ui_data.draw_time },
		{ "basic_shape_alpha_blend_scale",	guidance_ui_data.basic_shape_alpha_blend_scale },
		{ "basic_graphic_path",				guidance_ui_data.basic_graphic_path },
	};
}
#pragma endregion
