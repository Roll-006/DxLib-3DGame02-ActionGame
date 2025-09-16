#include "time_scale_controller.hpp"

TimeScaleController::TimeScaleController() : 
	m_next_time_scale{{ TimeScaleLayerKind::kNoneScale,	1.0f },
					  { TimeScaleLayerKind::kWorld,		1.0f },
					  { TimeScaleLayerKind::kPlayer,		1.0f },
					  { TimeScaleLayerKind::kUI,			1.0f },
					  { TimeScaleLayerKind::kAudio,		1.0f },
					  { TimeScaleLayerKind::kEffect,		1.0f },
					  { TimeScaleLayerKind::kCamera,		1.0f } }
{
	m_current_time_scale = m_next_time_scale;
}

TimeScaleController::~TimeScaleController()
{

}

void TimeScaleController::Update()
{
	m_current_time_scale = m_next_time_scale;
}

void TimeScaleController::OnNotify(const IEvent& event)
{
	// ロケットランチャー専用カットシーンの開始
	if (event.GetType() == std::type_index(typeid(StartRocketLauncherCutsceneData)))
	{
		const auto& event_data = static_cast<const Event<StartRocketLauncherCutsceneData>&>(event).data;
		m_next_time_scale.at(TimeScaleLayerKind::kWorld)		= event_data.world_time_scale;
		m_next_time_scale.at(TimeScaleLayerKind::kPlayer)	= event_data.player_time_scale;
		m_next_time_scale.at(TimeScaleLayerKind::kEffect)	= event_data.effect_time_scale;
	}
	// ロケットランチャー専用カットシーンの終了
	if (event.GetType() == std::type_index(typeid(EndRocketLauncherCutsceneData)))
	{
		const auto& event_data = static_cast<const Event<EndRocketLauncherCutsceneData>&>(event).data;
		m_next_time_scale.at(TimeScaleLayerKind::kWorld)		= event_data.time_scale;
		m_next_time_scale.at(TimeScaleLayerKind::kPlayer)	= event_data.time_scale;
		m_next_time_scale.at(TimeScaleLayerKind::kEffect)	= event_data.time_scale;
	}
}
