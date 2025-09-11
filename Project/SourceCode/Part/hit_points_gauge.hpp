#pragma once
#include "graphicer.hpp"
#include "hit_points.hpp"

class HitPointsGauge final
{
public:
	HitPointsGauge(const std::shared_ptr<HitPoints> hit_points);
	~HitPointsGauge();

private:
	std::shared_ptr<HitPoints> m_hit_points;
};
