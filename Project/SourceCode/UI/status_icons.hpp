#pragma once
#include "../Object/player.hpp"
#include "hit_points_gauge.hpp"

class StatusIcons final
{
public:
	StatusIcons(const std::shared_ptr<Player> player);
	~StatusIcons();

	void LateUpdate();
	void Draw() const;

private:
	std::shared_ptr<HitPointsGauge> m_hit_points_gauge;
};
