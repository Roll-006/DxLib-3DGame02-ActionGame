#include "sound_pool.hpp"

SoundPool::SoundPool()
{
	nlohmann::json pool_data;
	if (json_loader::Load("Data/JSON/sound_pool_data.json", pool_data))
	{
		nlohmann::json sound_data;
		if (json_loader::Load("Data/JSON/sound_data.json", sound_data))
		{
			const auto sound_json = sound_data.at("sound_data");
			for (const auto& value : sound_json)
			{
				const auto name = value.at("name").get<std::string>();
				const auto size = pool_data.at("sound_pool_data").at(name.c_str()).at("size").get<size_t>();
				for (size_t i = 0; i < size; ++i)
				{
					auto sound = std::make_shared<Sound>(value.get<SoundData>());
					m_sounds[sound->GetSoundData().name].push(sound);
				}
			}
		}
	}
}

SoundPool::~SoundPool()
{

}

void SoundPool::ReturnSound(const std::shared_ptr<Sound>& sound)
{
	if (!m_sounds.contains(sound->GetSoundData().name)) { return; }

	// 非アクティブ化して追加
	sound->Deactivate();
	m_sounds[sound->GetSoundData().name].push(sound);
}

std::shared_ptr<Sound> SoundPool::GetSound(const std::string& sound_name)
{
	if (!m_sounds.contains(sound_name))  { return nullptr; }
	if (m_sounds.at(sound_name).empty()) { return nullptr; }

	const auto sound = m_sounds.at(sound_name).front();
	m_sounds.at(sound_name).pop();

	// アクティブ化・初期化して取得
	sound->Activate();
	sound->Init();

	return sound;
}
