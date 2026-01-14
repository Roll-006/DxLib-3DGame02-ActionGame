#pragma once
#include "../Data/sound_volume_data.hpp"

class SoundVolumeSettings final
{
public:
	SoundVolumeSettings();
	~SoundVolumeSettings();

	void Update();

	void AllowChangeVolume();
	void DisallowChangeVolume();

	[[nodiscard]] const std::unordered_map<SoundKind, float>& GetVolumeAll() const { return data.volume; }
	[[nodiscard]] const float GetVolume(const SoundKind sound_kind) const { return data.volume.at(sound_kind); }

private:
	void ChangeSound(const SoundKind sound_kind);

private:
	SoundVolumeData data;
	bool m_can_change;
};
