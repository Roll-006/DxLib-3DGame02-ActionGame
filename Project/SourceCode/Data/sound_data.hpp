#pragma once
#include <nlohmann/json.hpp>
#include <string>

struct SoundData
{
	int			handle		= -1;

	std::string path		= "";
	std::string name		= "";
	int			priority	= 0;
	int			volume		= 255;
	float		radius		= 0.0f;
	bool		is_3d_sound = true;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, SoundData& sound_data)
{
	data.at("path")			.get_to(sound_data.path);
	data.at("name")			.get_to(sound_data.name);
	data.at("priority")		.get_to(sound_data.priority);
	data.at("volume")		.get_to(sound_data.volume);
	data.at("radius")		.get_to(sound_data.radius);
	data.at("is_3d_sound")	.get_to(sound_data.is_3d_sound);
}

inline void to_json(nlohmann::json& data, const SoundData& sound_data)
{
	data = nlohmann::json
	{
		{ "path",			sound_data.path },
		{ "name",			sound_data.name },
		{ "priority",		sound_data.priority },
		{ "volume",			sound_data.volume },
		{ "radius",			sound_data.radius },
		{ "is_3d_sound",	sound_data.is_3d_sound },
	};
}
#pragma endregion
