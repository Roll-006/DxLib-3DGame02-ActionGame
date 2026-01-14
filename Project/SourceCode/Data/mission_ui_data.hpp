#pragma once
#include "../Font/font_handler.hpp"
#include "../Part/mask_creator.hpp"
#include "../Part/screen_creator.hpp"
#include "../Data/text_data.hpp"

struct MissionUIData final
{
	Vector2D<int>	screen_size			= {};
	float			height_ratio		= 0.0f;
	TextData		text_data			= {};
	float			fade_in_speed		= 0.0f;
	float			fade_out_speed		= 0.0f;
	float			blend_wait_time		= 0.0f;		// アクティブ化されてから描画されるまでの待ち時間
	float			draw_time			= 0.0f;		// 描画時間
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, MissionUIData& mission_ui_data)
{
	j_data.at("screen_size")		.get_to(mission_ui_data.screen_size);
	j_data.at("height_ratio")		.get_to(mission_ui_data.height_ratio);
	j_data.at("text_data")			.get_to(mission_ui_data.text_data);
	j_data.at("fade_in_speed")		.get_to(mission_ui_data.fade_in_speed);
	j_data.at("fade_out_speed")		.get_to(mission_ui_data.fade_out_speed);
	j_data.at("blend_wait_time")	.get_to(mission_ui_data.blend_wait_time);
	j_data.at("draw_time")			.get_to(mission_ui_data.draw_time);
}

inline void to_json(nlohmann::json& j_data, const MissionUIData& mission_ui_data)
{
	j_data = nlohmann::json
	{
		{ "screen_size",		mission_ui_data.screen_size },
		{ "height_ratio",		mission_ui_data.height_ratio },
		{ "text_data",			mission_ui_data.text_data },
		{ "fade_in_speed",		mission_ui_data.fade_in_speed },
		{ "fade_out_speed",		mission_ui_data.fade_out_speed },
		{ "blend_wait_time",	mission_ui_data.blend_wait_time },
		{ "draw_time",			mission_ui_data.draw_time },
	};
}
#pragma endregion
