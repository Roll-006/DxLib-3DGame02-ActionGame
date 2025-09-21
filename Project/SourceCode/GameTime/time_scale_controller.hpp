#pragma once
#include "../Base/single_instance_base.hpp"
#include "../Event/event_system.hpp"

enum class TimeScaleLayerKind
{
	kNoneScale,	// 等倍
	kWorld,		// ゲーム全体(敵, 環境, 物理)
	kPlayer,	// プレイヤー
	kUI,		// UI
	kAudio,		// サウンド
	kEffect,	// エフェクト
	kCamera,	// カメラ
};

class TimeScaleController final : public SingleInstanceBase<TimeScaleController>
{
public:
	TimeScaleController();
	~TimeScaleController();

	void Update();


	#pragma region Event
	void SetStartRocketLauncherCutsceneTimeScale(const StartRocketLauncherCutsceneEvent& event);
	void SetEndRocketLauncherCutsceneTimeScale	(const EndRocketLauncherCutsceneEvent& event);
	#pragma endregion


	[[nodiscard]] float GetTimeScale(const TimeScaleLayerKind layer_kind) const { return m_current_time_scale.at(layer_kind); }

private:

private:
	std::unordered_map<TimeScaleLayerKind, float> m_current_time_scale;
	std::unordered_map<TimeScaleLayerKind, float> m_next_time_scale;
};
