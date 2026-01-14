#pragma once
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "../Kind/sound_kind.hpp"

struct SoundVolumeData
{
	std::unordered_map<SoundKind, float> volume = { {SoundKind::kMaster, 1.0f}, {SoundKind::kSE, 1.0f}, {SoundKind::kBGM, 1.0f} };
	float max_volume = 1.0f;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, SoundVolumeData& sound_volume_data)
{
	j_data.at("volume")			.get_to(sound_volume_data.volume);
	j_data.at("max_volume")		.get_to(sound_volume_data.max_volume);
}

inline void to_json(nlohmann::json& j_data, const SoundVolumeData& sound_volume_data)
{
	j_data = nlohmann::json
	{
		{ "volume",			sound_volume_data.volume },
		{ "max_volume",		sound_volume_data.max_volume },
	};
}
#pragma endregion
