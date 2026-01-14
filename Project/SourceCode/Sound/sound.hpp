#pragma once
#include "../Interface/i_poolable.hpp"
#include "../Data/sound_data.hpp"
#include "../GameTime/game_time_manager.hpp"
#include "../Calculation/math.hpp"

class Sound final : public IPoolable
{
public:
	Sound(const SoundData& j_data);
	~Sound();

	void Init();
	void Update(const float time_scale);

	void Activate()		{ m_is_active = true; }
	void Deactivate()	{ m_is_active = false; }

	/// @brief サウンドを再生する
	/// @param pos 3D空間での再生位置(3D空間でない場合は座標指定なし)
	void OnPlaySound(const TimeScaleLayerKind time_scale_layer, std::optional<VECTOR> pos = std::nullopt);

	/// @brief サウンドの再生を停止する
	void OnStopSound();

	/// @brief サウンドの再生を再開する
	void OnResumeSound();

	/// @brief フェードアウトを開始する
	/// @param fade_out_speed フェードアウト速度
	void StartFadeOut();

	[[nodiscard]] const bool		IsActive()		const		{ return m_is_active; }
	[[nodiscard]] const bool		IsStopping()	const		{ return m_is_stopping; }
	[[nodiscard]] const bool		IsReturnPool()	override	{ return m_is_return_pool; }
	[[nodiscard]] const SoundData	GetSoundData()	const		{ return sound_data; }

private:
	void FadeOut();

private:
	SoundData	sound_data;

	bool		m_is_active;
	bool		m_is_stopping;
	bool		m_is_fade_out;
	bool		m_is_return_pool;
	int			m_current_volume;

	friend void from_json(const nlohmann::json& j_data, Sound& sound);
	friend void to_json	 (nlohmann::json& j_data, const Sound& sound);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Sound& sound)
{
	j_data.at("sound_data").get_to(sound.sound_data);
}

inline void to_json(nlohmann::json& j_data, const Sound& sound)
{
	j_data = nlohmann::json
	{
		{ "sound_data", sound.sound_data },
	};
}
#pragma endregion
