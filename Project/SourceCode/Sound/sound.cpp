#include "sound.hpp"

Sound::Sound(const SoundData& j_data) : 
	sound_data			(j_data),
	m_is_active			(false),
	m_is_stopping		(true),
	m_is_fade_out		(false),
	m_is_return_pool	(false),
	m_current_volume	(j_data.volume)
{
	SetCreate3DSoundFlag(sound_data.is_3d_sound);
	sound_data.handle		= LoadSoundMem			(sound_data.path.c_str());
	sound_data.total_time	= GetSoundTotalTime		(sound_data.handle);
	sound_data.frequency	= GetFrequencySoundMem	(sound_data.handle);
	ChangeVolumeSoundMem(sound_data.volume, sound_data.handle);
	Set3DRadiusSoundMem (sound_data.radius, sound_data.handle);
}

Sound::~Sound()
{

}

void Sound::Init()
{
	m_is_stopping		= false;
	m_is_fade_out		= false;
	m_is_return_pool	= false;

	StopSoundMem(sound_data.handle);
	ChangeVolumeSoundMem(sound_data.volume, sound_data.handle);
}

void Sound::Update(const float time_scale)
{
	// 周波数をタイムスケールから変更
	const auto scale = std::clamp(time_scale, sound_data.min_frequency_scale, sound_data.max_frequency_scale);
	SetFrequencySoundMem(sound_data.frequency * scale, sound_data.handle);

	FadeOut();

	// 停止・再開処理
	time_scale <= 0.0f ? OnStopSound() : OnResumeSound();

	if (!m_is_return_pool) { m_is_return_pool = GetSoundCurrentTime(sound_data.handle) >= sound_data.total_time && !m_is_stopping; }
}

void Sound::OnPlaySound(const TimeScaleLayerKind time_scale_layer, std::optional<VECTOR> pos)
{
	if (!m_is_active) { return; }

	sound_data.time_scale_layer_kind = time_scale_layer;

	if (pos) { Set3DPositionSoundMem(*pos, sound_data.handle); }

	PlaySoundMem(sound_data.handle, sound_data.is_loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK, TRUE);
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

	PlaySoundMem(sound_data.handle, sound_data.is_loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK, FALSE);
}

void Sound::StartFadeOut()
{
	m_is_fade_out = true;
}

void Sound::FadeOut()
{
	if (!m_is_fade_out) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(sound_data.time_scale_layer_kind);
	math::Decrease(m_current_volume, static_cast<int>(sound_data.fade_out_speed * delta_time), 0);

	ChangeVolumeSoundMem(m_current_volume, sound_data.handle);

	// 音量が0になったらプールへ変換する
	m_is_return_pool = m_current_volume <= 0;
}
