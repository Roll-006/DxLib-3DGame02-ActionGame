#include "time_scale.hpp"

TimeScale::TimeScale() : 
	m_time_scale{{ LayerKind::kNoneScale,	1.0f },
				 { LayerKind::kWorld,		1.0f },
				 { LayerKind::kPlayer,		0.1f },
				 { LayerKind::kUI,			1.0f },
				 { LayerKind::kAudio,		1.0f },
				 { LayerKind::kEffect,		1.0f },
				 { LayerKind::kCamera,		1.0f } }
{

}

TimeScale::~TimeScale()
{

}

void TimeScale::OnNotify(const IEvent& event)
{

}
