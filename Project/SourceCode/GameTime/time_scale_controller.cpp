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
	if (event.GetType() == std::type_index(typeid(RocketLauncherShotData)))
	{
		const auto& rocket_launcher_shot = static_cast<const Event<RocketLauncherShotData>&>(event);
		m_next_time_scale.at(LayerKind::kWorld)  = rocket_launcher_shot.data.cutscene_time_scale;
		m_next_time_scale.at(LayerKind::kPlayer) = rocket_launcher_shot.data.cutscene_time_scale;
		m_next_time_scale.at(LayerKind::kEffect) = rocket_launcher_shot.data.cutscene_time_scale;
	}
}
