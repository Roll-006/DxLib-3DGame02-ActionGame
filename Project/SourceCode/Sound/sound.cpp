#include "sound.hpp"

Sound::Sound(const SoundData& data) : 
	sound_data			(data),
	m_is_active			(false),
	m_is_stopping		(true),
	m_current_play_time	(0ll),
	m_current_play_pos	(0ll)
{
	SetCreate3DSoundFlag(sound_data.is_3d_sound);
	sound_data.handle		= LoadSoundMem(sound_data.path.c_str());
	sound_data.total_time	= GetSoundTotalTime(sound_data.handle);
	sound_data.frequency	= GetFrequencySoundMem(sound_data.handle);
	ChangeVolumeSoundMem(sound_data.volume, sound_data.handle);
	Set3DRadiusSoundMem (sound_data.radius, sound_data.handle);
}

Sound::~Sound()
{

}

void Sound::Init()
{
	m_is_stopping = false;
}

void Sound::Update(const float time_scale)
{
	const auto scale = std::clamp(time_scale, sound_data.min_frequency_scale, sound_data.max_frequency_scale);

	SetFrequencySoundMem(sound_data.frequency * scale, sound_data.handle);

	time_scale <= 0.0f ? OnStopSound() : OnResumeSound();
}

void Sound::OnPlaySound(const TimeScaleLayerKind time_scale_layer, std::optional<VECTOR> pos)
{
	if (!m_is_active) { return; }

	sound_data.time_scale_layer_kind = time_scale_layer;

	if (pos) { Set3DPositionSoundMem(*pos, sound_data.handle); }

	PlaySoundMem(sound_data.handle, DX_PLAYTYPE_BACK, TRUE);
}

void Sound::OnStopSound()
{
	if (!m_is_active)  { return; }
	if (m_is_stopping) { return; }

	m_is_stopping = true;

	StopSoundMem(sound_data.handle);
}

void Sound::OnResumeSound()
{
	if (!m_is_active)	{ return; }
	if (!m_is_stopping) { return; }

	m_is_stopping = false;

	PlaySoundMem(sound_data.handle, DX_PLAYTYPE_BACK, FALSE);
}
