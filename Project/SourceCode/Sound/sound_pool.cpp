#include "sound_pool.hpp"

SoundPool::SoundPool()
{
	nlohmann::json data;
	if (json_loader::Load("Data/JSON/sound_data.json", data))
	{
		const auto sound_json = data.at("sound_data");
		for (const auto& value : sound_json)
		{
			auto sound = std::make_shared<Sound>(value.get<SoundData>());
			m_sounds[sound->GetSoundData().name].push(sound);
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
