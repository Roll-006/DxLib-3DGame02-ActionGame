#include "time_scale_controller.hpp"

TimeScaleController::TimeScaleController() : 
	m_next_time_scale{{ LayerKind::kNoneScale,	1.0f },
					  { LayerKind::kWorld,		1.0f },
					  { LayerKind::kPlayer,		1.0f },
					  { LayerKind::kUI,			1.0f },
					  { LayerKind::kAudio,		1.0f },
					  { LayerKind::kEffect,		1.0f },
					  { LayerKind::kCamera,		1.0f } }
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
		m_next_time_scale.at(LayerKind::kWorld)		= event_data.world_time_scale;
		m_next_time_scale.at(LayerKind::kPlayer)	= event_data.player_time_scale;
		m_next_time_scale.at(LayerKind::kEffect)	= event_data.effect_time_scale;
	}
	// ロケットランチャー専用カットシーンの終了
	if (event.GetType() == std::type_index(typeid(EndRocketLauncherCutsceneData)))
	{
		const auto& event_data = static_cast<const Event<EndRocketLauncherCutsceneData>&>(event).data;
		m_next_time_scale.at(LayerKind::kWorld)		= event_data.time_scale;
		m_next_time_scale.at(LayerKind::kPlayer)	= event_data.time_scale;
		m_next_time_scale.at(LayerKind::kEffect)	= event_data.time_scale;
	}
}
