#include "sound_player.hpp"

SoundPlayer::SoundPlayer() : 
	m_sound_pool(std::make_unique<SoundPool>())
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<WeaponShotEvent>		(this, &SoundPlayer::PlayWeaponShotSound);
	EventSystem::GetInstance()->Subscribe<DropShellCasing>		(this, &SoundPlayer::PlayDropShellCasingSound);
	EventSystem::GetInstance()->Subscribe<OnGroundFootEvent>	(this, &SoundPlayer::PlayOnGroundFootSound);
	EventSystem::GetInstance()->Subscribe<EmptyAmmoEvent>		(this, &SoundPlayer::PlayEmptyAmmoSound);
	EventSystem::GetInstance()->Subscribe<AimGunEvent>			(this, &SoundPlayer::PlayAimGunSound);
	EventSystem::GetInstance()->Subscribe<ExitAimGunEvent>		(this, &SoundPlayer::PlayExitAimGunSound);
	EventSystem::GetInstance()->Subscribe<SetAmmoBoxEvent>		(this, &SoundPlayer::PlaySetAmmoBoxSound);
	EventSystem::GetInstance()->Subscribe<ReleaseAmmoBoxEvent>	(this, &SoundPlayer::PlayReleaseAmmoBoxSound);
	EventSystem::GetInstance()->Subscribe<CockingEvent>			(this, &SoundPlayer::PlayCockingSound);
	EventSystem::GetInstance()->Subscribe<OnHitKickEvent>		(this, &SoundPlayer::PlayOnHitKickSound);
	EventSystem::GetInstance()->Subscribe<PickUpItemEvent>		(this, &SoundPlayer::PlayPickUpItemSound);
	EventSystem::GetInstance()->Subscribe<OnDamageEvent>		(this, &SoundPlayer::PlayOnDamageSound);
	EventSystem::GetInstance()->Subscribe<OnHitBulletEvent>		(this, &SoundPlayer::PlayOnHitBulletSound);
}

SoundPlayer::~SoundPlayer()
{
	// イベント登録
	EventSystem::GetInstance()->Unsubscribe<WeaponShotEvent>	(this, &SoundPlayer::PlayWeaponShotSound);
	EventSystem::GetInstance()->Unsubscribe<DropShellCasing>	(this, &SoundPlayer::PlayDropShellCasingSound);
	EventSystem::GetInstance()->Unsubscribe<OnGroundFootEvent>	(this, &SoundPlayer::PlayOnGroundFootSound);
	EventSystem::GetInstance()->Unsubscribe<EmptyAmmoEvent>		(this, &SoundPlayer::PlayEmptyAmmoSound);
	EventSystem::GetInstance()->Unsubscribe<AimGunEvent>		(this, &SoundPlayer::PlayAimGunSound);
	EventSystem::GetInstance()->Unsubscribe<ExitAimGunEvent>	(this, &SoundPlayer::PlayExitAimGunSound);
	EventSystem::GetInstance()->Unsubscribe<SetAmmoBoxEvent>	(this, &SoundPlayer::PlaySetAmmoBoxSound);
	EventSystem::GetInstance()->Unsubscribe<ReleaseAmmoBoxEvent>(this, &SoundPlayer::PlayReleaseAmmoBoxSound);
	EventSystem::GetInstance()->Unsubscribe<CockingEvent>		(this, &SoundPlayer::PlayCockingSound);
	EventSystem::GetInstance()->Unsubscribe<OnHitKickEvent>		(this, &SoundPlayer::PlayOnHitKickSound);
	EventSystem::GetInstance()->Unsubscribe<PickUpItemEvent>	(this, &SoundPlayer::PlayPickUpItemSound);
	EventSystem::GetInstance()->Unsubscribe<OnDamageEvent>		(this, &SoundPlayer::PlayOnDamageSound);
	EventSystem::GetInstance()->Unsubscribe<OnHitBulletEvent>	(this, &SoundPlayer::PlayOnHitBulletSound);
}

void SoundPlayer::Update()
{
	Set3DSoundListenerPosAndFrontPos_UpVecY(GetCameraPosition(), GetCameraFrontVector());

	auto remove_sounds = std::vector<std::shared_ptr<Sound>>();

	const auto time_manager = GameTimeManager::GetInstance();
	for (const auto& [name, sounds] : m_active_sounds)
	{
		for (const auto& sound : sounds)
		{
			sound->Update(time_manager->GetTimeScale(sound->GetSoundData().time_scale_layer_kind));

			// プールに返却
			if (sound->IsReturnPool())
			{
				m_sound_pool->ReturnSound(sound);
				remove_sounds.emplace_back(sound);
			}
		}
	}

	RemoveSounds(remove_sounds);
}

void SoundPlayer::RemoveSounds(std::vector<std::shared_ptr<Sound>>& remove_sounds)
{
	for (const auto& sound : remove_sounds)
	{
		const auto name = sound->GetSoundData().name;
		if (std::find(m_active_sounds.at(name).begin(), m_active_sounds.at(name).end(), sound) != m_active_sounds.at(name).end())
		{
			erase(m_active_sounds.at(name), sound);
		}
	}
}

void SoundPlayer::OnPlaySound(const std::string& sound_name, const TimeScaleLayerKind time_scale_layer, std::optional<VECTOR> pos)
{
	// プールから取得して再生
	auto sound = m_sound_pool->GetSound(sound_name);

	// プールが空であった場合、再生中のサウンドで最も古いサウンドを再利用
	if (!sound)
	{
		const auto active_sound = m_active_sounds.at(sound_name).front();
		active_sound->OnStopSound();
		m_sound_pool->ReturnSound(active_sound);
		if (std::find(m_active_sounds.at(sound_name).begin(), m_active_sounds.at(sound_name).end(), active_sound) != m_active_sounds.at(sound_name).end())
		{
			erase(m_active_sounds.at(sound_name), active_sound);
		}

		sound = m_sound_pool->GetSound(sound_name);
		if (!sound) { return; }
	}

	// 3D空間のサウンドで、聴こえる範囲外であれば再生しない
	if (pos)
	{
		if (VSize(GetCameraPosition() - *pos) > sound->GetSoundData().radius)
		{
			m_sound_pool->ReturnSound(sound);
			return;
		}
	}

	m_active_sounds[sound->GetSoundData().name].emplace_back(sound);
	sound->OnPlaySound(time_scale_layer, pos);
}


#pragma region Event
void SoundPlayer::PlayWeaponShotSound		(const WeaponShotEvent&		event)
{
	switch (event.gun_kind)
	{
	case GunKind::kAssaultRifle:
		OnPlaySound("shot_assault_rifle",   event.time_scale_layer_kind, event.muzzle_transform->GetPos(CoordinateKind::kWorld));
		break;

	case GunKind::kRocketLauncher:
		OnPlaySound("shot_rocket_launcher", event.time_scale_layer_kind, event.muzzle_transform->GetPos(CoordinateKind::kWorld));
		break;

	default:
		break;
	}
}

void SoundPlayer::PlayDropShellCasingSound	(const DropShellCasing&		event)
{
	OnPlaySound("drop_shell_casing", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlayOnGroundFootSound		(const OnGroundFootEvent&	event)
{
	if (event.is_run)
	{
		const auto name = event.is_left_foot ? "run_left_foot_on_lawn"  : "run_right_foot_on_lawn";
		OnPlaySound(name, event.time_scale_layer_kind, event.pos);
	}
	else
	{
		const auto name = event.is_left_foot ? "walk_left_foot_on_lawn" : "walk_right_foot_on_lawn";
		OnPlaySound(name, event.time_scale_layer_kind, event.pos);
	}
}

void SoundPlayer::PlayEmptyAmmoSound		(const EmptyAmmoEvent&		event)
{
	OnPlaySound("empty_ammo", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlayAimGunSound			(const AimGunEvent&			event)
{
	OnPlaySound("aim_gun", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlayExitAimGunSound		(const ExitAimGunEvent&		event)
{
	OnPlaySound("exit_aim_gun", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlaySetAmmoBoxSound		(const SetAmmoBoxEvent&		event)
{
	OnPlaySound("set_ammo_box", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlayReleaseAmmoBoxSound	(const ReleaseAmmoBoxEvent& event)
{
	OnPlaySound("release_ammo_box", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlayCockingSound			(const CockingEvent&		event)
{
	OnPlaySound("cocking", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlayOnHitKickSound		(const OnHitKickEvent&		event)
{
	OnPlaySound("hit_kick", event.time_scale_layer_kind, event.pos);
}

void SoundPlayer::PlayPickUpItemSound		(const PickUpItemEvent&		event)
{
	switch (event.item_kind)
	{
	case ItemKind::kWeapon:
		break;

	case ItemKind::kAmmoBox:
		OnPlaySound("pick_up_item", event.time_scale_layer_kind, event.owner_pos);
		break;

	case ItemKind::kPotion:
		break;

	default:
		break;
	}
}

void SoundPlayer::PlayOnDamageSound			(const OnDamageEvent&		event)
{
	OnPlaySound("bleed", event.time_scale_layer_kind, event.hit_pos);
}

void SoundPlayer::PlayOnHitBulletSound		(const OnHitBulletEvent&	event)
{
	if (event.bullet_name == ObjName.ROCKET_BOMB)
	{
		OnPlaySound("explosion", event.time_scale_layer_kind, event.hit_pos);
	}
}
#pragma endregion
