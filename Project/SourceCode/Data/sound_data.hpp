#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <DxLib.h>
#include "../Kind/time_scale_layer_kind.hpp"
#include "../Kind/sound_kind.hpp"

struct SoundData
{
	int					handle					= -1;
	LONGLONG			total_time				= 0ll;
	int					frequency				= 0;

	std::string			path					= "";
	std::string			name					= "";
	int					priority				= 0;
	int					volume					= 255;
	float				radius					= 0.0f;
	float				fade_out_speed			= 0.0f;
	float				max_frequency_scale		= 1.0f;
	float				min_frequency_scale		= 0.0f;
	bool				is_3d_sound				= true;
	bool				is_loop					= false;
	SoundKind			sound_kind				= SoundKind::kSE;
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, SoundData& sound_data)
{
	j_data.at("path")					.get_to(sound_data.path);
	j_data.at("name")					.get_to(sound_data.name);
	j_data.at("priority")				.get_to(sound_data.priority);
	j_data.at("volume")					.get_to(sound_data.volume);
	j_data.at("radius")					.get_to(sound_data.radius);
	j_data.at("fade_out_speed")			.get_to(sound_data.fade_out_speed);
	j_data.at("max_frequency_scale")	.get_to(sound_data.max_frequency_scale);
	j_data.at("min_frequency_scale")	.get_to(sound_data.min_frequency_scale);
	j_data.at("is_3d_sound")			.get_to(sound_data.is_3d_sound);
	j_data.at("is_loop")				.get_to(sound_data.is_loop);
	j_data.at("sound_kind")				.get_to(sound_data.sound_kind);
	j_data.at("time_scale_layer_kind")	.get_to(sound_data.time_scale_layer_kind);
}

inline void to_json(nlohmann::json& j_data, const SoundData& sound_data)
{
	j_data = nlohmann::json
	{
		{ "path",					sound_data.path },
		{ "name",					sound_data.name },
		{ "priority",				sound_data.priority },
		{ "volume",					sound_data.volume },
		{ "radius",					sound_data.radius },
		{ "fade_out_speed",			sound_data.fade_out_speed },
		{ "max_frequency_scale",	sound_data.max_frequency_scale },
		{ "min_frequency_scale",	sound_data.min_frequency_scale },
		{ "is_3d_sound",			sound_data.is_3d_sound },
		{ "is_loop",				sound_data.is_loop },
		{ "sound_kind",				sound_data.sound_kind },
		{ "time_scale_layer_kind",	sound_data.time_scale_layer_kind },
	};
}
#pragma endregion
