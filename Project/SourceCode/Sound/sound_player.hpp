#pragma once
#include "../Event/event_system.hpp"
#include "../JSON/json_loader.hpp"

class SoundPlayer final
{
public:
	SoundPlayer();
	~SoundPlayer();

private:
	#pragma region Event

	#pragma endregion

private:
	std::unordered_map<std::string, int> m_sound_handle;
};