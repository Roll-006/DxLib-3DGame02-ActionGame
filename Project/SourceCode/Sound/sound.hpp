#pragma once
#include <DxLib.h>
#include "../Data/sound_data.hpp"

class Sound final
{
public:
	Sound(const SoundData& data);
	~Sound();

	void Init();

	void Activate();
	void Deactivate();

	void OnPlaySound(std::optional<VECTOR> pos = std::nullopt);

	[[nodiscard]] bool		IsActive()		const { return m_is_active; }
	[[nodiscard]] bool		IsPlaying()		const { return CheckSoundMem(sound_data.handle); }
	[[nodiscard]] SoundData GetSoundData()	const { return sound_data; }

private:
	SoundData sound_data;

	bool m_is_active;

	friend void from_json(const nlohmann::json& data, Sound& sound);
	friend void to_json	 (nlohmann::json& data, const Sound& sound);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, Sound& sound)
{
	data.at("sound_data").get_to(sound.sound_data);
}

inline void to_json(nlohmann::json& data, const Sound& sound)
{
	data = nlohmann::json
	{
		{ "sound_data", sound.sound_data },
	};
}
#pragma endregion
