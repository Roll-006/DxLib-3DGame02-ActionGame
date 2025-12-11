#pragma once
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

	/// @brief サウンド再生
	void OnPlaySound(const std::string& sound_name, const TimeScaleLayerKind time_scale_layer, std::optional<VECTOR> pos = std::nullopt);

	/// @brief フェードアウトを行う
	/// @brief 完了した場合、サウンドはプールに変換させる
	void OnFadeOut(const std::string& sound_name);


	#pragma region Event
	void PlayChangeSceneSound		(const ChangeSceneEvent&		event);
	void PlayWeaponShotSound		(const WeaponShotEvent&			event);
	void PlayDropShellCasingSound	(const DropShellCasing&			event);
	void PlayOnGroundFootSound		(const OnGroundFootEvent&		event);
	void PlayEmptyAmmoSound			(const EmptyAmmoEvent&			event);
	void PlayAimGunSound			(const AimGunEvent&				event);
	void PlayExitAimGunSound		(const ExitAimGunEvent&			event);
	void PlaySetAmmoBoxSound		(const SetAmmoBoxEvent&			event);
	void PlayReleaseAmmoBoxSound	(const ReleaseAmmoBoxEvent&		event);
	void PlayCockingSound			(const CockingEvent&			event);
	void PlayOnHitKickSound			(const OnHitKickEvent&			event);
	void PlayPickUpItemSound		(const PickUpItemEvent&			event);
	void PlayOnDamageSound			(const OnDamageEvent&			event);
	void PlayOnHitBulletSound		(const OnHitBulletEvent&		event);
	void PlayDecisionSound			(const DecisionEvent&			event);
	void PlayBackSound				(const BackEvent&				event);
	void PlaySelectMenuSound		(const SelectMenuEvent&			event);
	void PlayOpenPageSound			(const OpenPageEvent&			event);
	void PlayBattleSound			(const StartBattleEvent&		event);
	void PlayStageSound				(const EndBattleEvent&			event);
	void PlayGameClearSound			(const DeadAllEnemyEvent&		event);
	void PlayGameOverSound			(const DeadPlayerEvent&			event);
	void PlayStabKnifeSound			(const StabKnifeEvent&			event);
	void PlayDrawKnifeSound			(const DrawKnifeEvent&			event);
	void PlayFirstSlashSound		(const FirstSlashEvent&			event);
	void PlaySecondSlashSound		(const SecondSlashEvent&		event);
	void PlaySpinningSlashSound		(const SpinningSlashEvent&		event);
	void PlayShoutSound				(const OnTargetDetectedEvent&	event);
	void PlayBiteZombieSound		(const GrabEvent&				event);
	void PlayStunZombieSound		(const StunEvent&				event);
	
	void StopBiteZombieSound		(const ReleaseEvent&			event);
	void StopStunZombieSound		(const ExitStunEvent&			event);
	#pragma endregion

private:
	std::unique_ptr<SoundPool> m_sound_pool;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Sound>>> m_active_sounds;
};
