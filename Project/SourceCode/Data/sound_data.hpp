#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <DxLib.h>
#include "../Kind/time_scale_layer_kind.hpp"

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
	float				max_frequency_scale		= 1.0f;
	float				min_frequency_scale		= 0.0f;
	bool				is_3d_sound				= true;
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, SoundData& sound_data)
{
	data.at("path")					.get_to(sound_data.path);
	data.at("name")					.get_to(sound_data.name);
	data.at("priority")				.get_to(sound_data.priority);
	data.at("volume")				.get_to(sound_data.volume);
	data.at("radius")				.get_to(sound_data.radius);
	data.at("max_frequency_scale")	.get_to(sound_data.max_frequency_scale);
	data.at("min_frequency_scale")	.get_to(sound_data.min_frequency_scale);
	data.at("is_3d_sound")			.get_to(sound_data.is_3d_sound);
	data.at("time_scale_layer_kind").get_to(sound_data.time_scale_layer_kind);
}

inline void to_json(nlohmann::json& data, const SoundData& sound_data)
{
	data = nlohmann::json
	{
		{ "path",					sound_data.path },
		{ "name",					sound_data.name },
		{ "priority",				sound_data.priority },
		{ "volume",					sound_data.volume },
		{ "radius",					sound_data.radius },
		{ "max_frequency_scale",	sound_data.max_frequency_scale },
		{ "min_frequency_scale",	sound_data.min_frequency_scale },
		{ "is_3d_sound",			sound_data.is_3d_sound },
		{ "time_scale_layer_kind",	sound_data.time_scale_layer_kind },
	};
}
#pragma endregion
