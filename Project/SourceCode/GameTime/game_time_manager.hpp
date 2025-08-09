#pragma once
#include "../Base/singleton_base.hpp"
#include "fps.hpp"
#include "time_scale_controller.hpp"

class GameTimeManager final : public SingletonBase<GameTimeManager>
{
public:
	void Update();
	void Draw();
	void WaitTime();

	[[nodiscard]] float GetDeltaTime(const TimeScaleController::LayerKind scale_layer_kind) const;

	[[nodiscard]] std::shared_ptr<TimeScaleController> GetTimeScaleController() const
	{
		return m_time_scale_controller;
	}

private:
	GameTimeManager();
	~GameTimeManager() override;

private:
	std::unique_ptr<FPS>				 m_fps;
	std::shared_ptr<TimeScaleController> m_time_scale_controller;

	friend SingletonBase<GameTimeManager>;
};
