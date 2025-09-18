#include "time_scale_controller.hpp"

TimeScaleController::TimeScaleController() : 
	m_next_time_scale{{ TimeScaleLayerKind::kNoneScale,	1.0f },
					  { TimeScaleLayerKind::kWorld,		1.0f },
					  { TimeScaleLayerKind::kPlayer,	1.0f },
					  { TimeScaleLayerKind::kUI,		1.0f },
					  { TimeScaleLayerKind::kAudio,		1.0f },
					  { TimeScaleLayerKind::kEffect,	1.0f },
					  { TimeScaleLayerKind::kCamera,	1.0f } }
{
	m_current_time_scale = m_next_time_scale;

	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<StartRocketLauncherCutsceneEvent>([this](const StartRocketLauncherCutsceneEvent& event)
	{
		SetStartRocketLauncherCutsceneTimeScale(event);
	});
	EventSystem::GetInstance()->Subscribe<EndRocketLauncherCutsceneEvent>  ([this](const EndRocketLauncherCutsceneEvent&   event)
	{
		SetEndRocketLauncherCutsceneTimeScale(event);
	});
}

TimeScaleController::~TimeScaleController()
{

}

void TimeScaleController::Update()
{
	m_current_time_scale = m_next_time_scale;
}

void TimeScaleController::SetStartRocketLauncherCutsceneTimeScale(const StartRocketLauncherCutsceneEvent& event)
{
	m_next_time_scale.at(TimeScaleLayerKind::kWorld)  = event.world_time_scale;
	m_next_time_scale.at(TimeScaleLayerKind::kPlayer) = event.player_time_scale;
	m_next_time_scale.at(TimeScaleLayerKind::kEffect) = event.effect_time_scale;
}

void TimeScaleController::SetEndRocketLauncherCutsceneTimeScale(const EndRocketLauncherCutsceneEvent& event)
{
	m_next_time_scale.at(TimeScaleLayerKind::kWorld)  = event.time_scale;
	m_next_time_scale.at(TimeScaleLayerKind::kPlayer) = event.time_scale;
	m_next_time_scale.at(TimeScaleLayerKind::kEffect) = event.time_scale;
}
