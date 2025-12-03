#include "sound_player.hpp"

SoundPlayer::SoundPlayer()
{
	nlohmann::json data;
	if (json_loader::Load("Data/JSON/sound_path.json", data))
	{
		const auto sound_json = data.at("sound_data");
		for (const auto [sound_name, path] : sound_json.items())
		{
			m_sound_handle[sound_name] = LoadSoundMem(path.get<std::string>().c_str());
		}
	}
}

SoundPlayer::~SoundPlayer()
{

}
