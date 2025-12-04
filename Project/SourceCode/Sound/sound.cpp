#include "sound.hpp"

Sound::Sound(const SoundData& data) : 
	sound_data	(data),
	m_is_active	(false)
{
	SetCreate3DSoundFlag(sound_data.is_3d_sound);
	sound_data.handle = LoadSoundMem(sound_data.path.c_str());
	ChangeVolumeSoundMem(sound_data.volume, sound_data.handle);
	Set3DRadiusSoundMem (sound_data.radius, sound_data.handle);
}

Sound::~Sound()
{

}

void Sound::Init()
{
	
}

void Sound::Activate()
{
	m_is_active = true;
}

void Sound::Deactivate()
{
	m_is_active = false;
}

void Sound::OnPlaySound(std::optional<VECTOR> pos)
{
	if (!m_is_active) { return; }

	if (pos) { Set3DPositionSoundMem(*pos, sound_data.handle); }

	PlaySoundMem(sound_data.handle, DX_PLAYTYPE_BACK);
}
