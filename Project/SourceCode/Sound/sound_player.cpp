#include "sound_player.hpp"

SoundPlayer::SoundPlayer() : 
	m_sound_pool(std::make_unique<SoundPool>())
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<WeaponShotEvent>	(this, &SoundPlayer::OutputWeaponShotSound);
	EventSystem::GetInstance()->Subscribe<DropShellCasing>	(this, &SoundPlayer::OutputDropShellCasingSound);
	EventSystem::GetInstance()->Subscribe<OnGroundFoot>		(this, &SoundPlayer::OutputOnGroundFootSound);
}

SoundPlayer::~SoundPlayer()
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Unsubscribe<WeaponShotEvent>(this, &SoundPlayer::OutputWeaponShotSound);
	EventSystem::GetInstance()->Unsubscribe<DropShellCasing>(this, &SoundPlayer::OutputDropShellCasingSound);
	EventSystem::GetInstance()->Unsubscribe<OnGroundFoot>	(this, &SoundPlayer::OutputOnGroundFootSound);
}

void SoundPlayer::Update()
{
	Set3DSoundListenerPosAndFrontPos_UpVecY(GetCameraPosition(), GetCameraFrontVector());

	for (const auto& [name, sounds] : m_active_sounds)
	{
		for (const auto& sound : sounds)
		{
			if (sound->IsPlaying()) { continue; }

			m_sound_pool->ReturnSound(sound);
		}
	}
}


#pragma region Event
void SoundPlayer::OutputWeaponShotSound		(const WeaponShotEvent& event)
{
	std::shared_ptr<Sound> sound = nullptr;

	switch (event.gun_kind)
	{
	case GunKind::kAssaultRifle:
		sound = m_sound_pool->GetSound("shot_assault_rifle");
		if (sound)
		{
			if (VSize(GetCameraPosition() - event.muzzle_transform->GetPos(CoordinateKind::kWorld)) > sound->GetSoundData().radius)
			{
				m_sound_pool->ReturnSound(sound);
				return;
			}

			m_active_sounds[sound->GetSoundData().name].emplace_back(sound);
			sound->OnPlaySound(event.muzzle_transform->GetPos(CoordinateKind::kWorld));
		}
		break;

	case GunKind::kRocketLauncher:
		break;

	default:
		break;
	}
}

void SoundPlayer::OutputDropShellCasingSound(const DropShellCasing& event)
{
	const auto sound = m_sound_pool->GetSound("drop_shell_casing");
	if (sound)
	{
		if (VSize(GetCameraPosition() - event.pos) > sound->GetSoundData().radius)
		{
			m_sound_pool->ReturnSound(sound);
			return;
		}

		m_active_sounds[sound->GetSoundData().name].emplace_back(sound);
		sound->OnPlaySound(event.pos);
	}
}

void SoundPlayer::OutputOnGroundFootSound	(const OnGroundFoot&	event)
{
	auto name = "";

	if (event.is_run)
	{
		name = event.is_left_foot ? "run_left_foot_on_lawn"  : "run_right_foot_on_lawn";
	}
	else
	{
		name = event.is_left_foot ? "walk_left_foot_on_lawn" : "walk_right_foot_on_lawn";
	}

	const auto sound = m_sound_pool->GetSound(name);
	if (sound)
	{
		if (VSize(GetCameraPosition() - event.pos) > sound->GetSoundData().radius)
		{
			m_sound_pool->ReturnSound(sound);
			return;
		}

		m_active_sounds[sound->GetSoundData().name].emplace_back(sound);
		sound->OnPlaySound(event.pos);
	}
}
#pragma endregion
