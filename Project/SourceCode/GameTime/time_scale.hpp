#pragma once
#include "../Interface/i_observer.hpp"

class TimeScale final : public OneInstanceSingletonBase<TimeScale>, public IObserver
{
public:
	enum class LayerKind
	{
		kNoneScale,	// 等倍
		kWorld,		// ゲーム全体(敵, 環境, 物理)
		kPlayer,	// プレイヤー
		kUI,		// UI
		kAudio,		// サウンド
		kEffect,	// エフェクト
		kCamera,	// カメラ
	};

public:
	TimeScale();
	~TimeScale();

	void OnNotify(const IEvent& event) override;

	[[nodiscard]] float GetTimeScale(const LayerKind layer_kind) const { return m_time_scale.at(layer_kind); }

private:
	std::unordered_map<LayerKind, float> m_time_scale;
};
