#pragma once
#include "../Base/singleton_base.hpp"
#include "fps.hpp"
#include "time_scale.hpp"

class GameTimeManager final : public SingletonBase<GameTimeManager>
{
public:
	void Update();
	void Draw();
	void WaitTime();

	[[nodiscard]] float GetDeltaTime(const TimeScale::LayerKind scale_layer_kind) const;

private:
	GameTimeManager();
	~GameTimeManager() override;

private:
	std::unique_ptr<FPS>		m_fps;
	std::shared_ptr<TimeScale>	m_time_scale;

	friend SingletonBase<GameTimeManager>;
};
