#pragma once
#include "../Event/event_system.hpp"
#include "sound_pool.hpp"

class SoundPlayer final
{
public:
	SoundPlayer();
	~SoundPlayer();

	void Update();

private:
	#pragma region Event
	void OutputWeaponShotSound		(const WeaponShotEvent& event);
	void OutputDropShellCasingSound	(const DropShellCasing& event);
	void OutputOnGroundFootSound	(const OnGroundFoot&	event);
	#pragma endregion

private:
	std::unique_ptr<SoundPool> m_sound_pool;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sound>>> m_active_sounds;
};