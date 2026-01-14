#include "../JSON/json_loader.hpp"
#include "../Event/event_system.hpp"
#include "sound_volume_settings.hpp"

SoundVolumeSettings::SoundVolumeSettings() : 
	m_can_change(false)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/sound_volume.json", j_data))
	{
		const auto sound_volume = j_data.at("sound_volume");
		data.max_volume = sound_volume.at("max_volume");
		for (const auto& [kind, vol] : sound_volume.at("volume").items())
		{
			data.volume[static_cast<SoundKind>(atoi(kind.c_str()))] = vol.get<int>();
		}
	}
}

SoundVolumeSettings::~SoundVolumeSettings()
{

}

void SoundVolumeSettings::Update()
{

}

void SoundVolumeSettings::AllowChangeVolume()
{
	m_can_change = true;
}

void SoundVolumeSettings::DisallowChangeVolume()
{
	m_can_change = false;
}

void SoundVolumeSettings::ChangeSound(const SoundKind sound_kind)
{
	// âºÇ≈âπó ÇïœçX
	//if()
}
