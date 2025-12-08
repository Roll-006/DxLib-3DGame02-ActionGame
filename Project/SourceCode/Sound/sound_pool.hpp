#pragma once
#include <queue>

#include "sound.hpp"
#include "../JSON/json_loader.hpp"

class SoundPool final
{
public:
	SoundPool();
	~SoundPool();

	/// @brief 使用後のサウンドを返却する
	/// @brief 自動的に非アクティブ化される
	void ReturnSound(const std::shared_ptr<Sound>& sound);

	/// @brief 使用可能なサウンドを取得する
	/// @return 初期化・アクティブ化されたサウンド(使用可能なサウンドがない場合はnullptr)
	[[nodiscard]] std::shared_ptr<Sound> GetSound(const std::string& sound_name);

private:
	std::unordered_map<std::string, std::queue<std::shared_ptr<Sound>>> m_sounds;
};
