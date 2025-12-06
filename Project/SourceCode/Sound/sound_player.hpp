#pragma once
#include "../GameTime/game_time_manager.hpp"
#include "../Event/event_system.hpp"
#include "../Name/obj_name.hpp"
#include "sound_pool.hpp"

class SoundPlayer final
{
public:
	SoundPlayer();
	~SoundPlayer();

	void Update();

private:
	void RemoveSounds(std::vector<std::shared_ptr<Sound>>& remove_sounds);

	void OnPlaySound(const std::string& sound_name, const TimeScaleLayerKind time_scale_layer, std::optional<VECTOR> pos = std::nullopt);


	#pragma region Event
	void PlayWeaponShotSound		(const WeaponShotEvent&		event);
	void PlayDropShellCasingSound	(const DropShellCasing&		event);
	void PlayOnGroundFootSound		(const OnGroundFootEvent&	event);
	void PlayEmptyAmmoSound			(const EmptyAmmoEvent&		event);
	void PlayAimGunSound			(const AimGunEvent&			event);
	void PlayExitAimGunSound		(const ExitAimGunEvent&		event);
	void PlaySetAmmoBoxSound		(const SetAmmoBoxEvent&		event);
	void PlayReleaseAmmoBoxSound	(const ReleaseAmmoBoxEvent& event);
	void PlayCockingSound			(const CockingEvent&		event);
	void PlayOnHitKickSound			(const OnHitKickEvent&		event);
	void PlayPickUpItemSound		(const PickUpItemEvent&		event);
	void PlayOnDamageSound			(const OnDamageEvent&		event);
	void PlayOnHitBulletSound		(const OnHitBulletEvent&	event);
	void PlayDecisionSound			(const DecisionEvent&		event);
	void PlayBackSound				(const BackEvent&			event);
	void PlaySelectMenuSound		(const SelectMenuEvent&		event);
	void PlayOpenPageSound			(const OpenPageEvent&		event);
	#pragma endregion

private:
	std::unique_ptr<SoundPool> m_sound_pool;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sound>>> m_active_sounds;
};
