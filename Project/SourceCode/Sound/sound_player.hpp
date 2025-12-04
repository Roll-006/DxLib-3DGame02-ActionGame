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
	void OutputWeaponShotSound		(const WeaponShotEvent&		event);
	void OutputDropShellCasingSound	(const DropShellCasing&		event);
	void OutputOnGroundFootSound	(const OnGroundFootEvent&	event);
	void OutputEmptyAmmoSound		(const EmptyAmmoEvent&		event);
	void OutputAimGunSound			(const AimGunEvent&			event);
	void OutputSetAmmoBoxSound		(const SetAmmoBoxEvent&		event);
	void OutputReleaseAmmoBoxSound	(const ReleaseAmmoBoxEvent& event);
	void OutputCockingSound			(const CockingEvent&		event);
	#pragma endregion

private:
	std::unique_ptr<SoundPool> m_sound_pool;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sound>>> m_active_sounds;
};