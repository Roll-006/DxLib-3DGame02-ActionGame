#pragma once
#include "../Interface/i_observer.hpp"

#include "../Part/subject.hpp"

class TimeScaleController final : public OneInstanceSingletonBase<TimeScaleController>, public IObserver
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
	TimeScaleController();
	~TimeScaleController();

	void Update();

	void OnNotify(const IEvent& event) override;

	template<typename T>
	void AddToSubject(const std::shared_ptr<Subject<T>> subject)
	{
		if (subject != nullptr)
		{
			// shared_ptrでラップして渡す（カスタムデリーター付き）
			std::shared_ptr<IObserver> observer = std::shared_ptr<IObserver>(this, [](IObserver*) {});
			subject->AddObserver(observer);
		}
	}

	[[nodiscard]] float GetTimeScale(const LayerKind layer_kind) const { return m_current_time_scale.at(layer_kind); }

private:
	std::unordered_map<LayerKind, float> m_current_time_scale;
	std::unordered_map<LayerKind, float> m_next_time_scale;
};
