#pragma once
#include "../Part/screen_creator.hpp"
#include "../Part/hit_points.hpp"

class HitPointsGauge final
{
public:
	HitPointsGauge(std::shared_ptr<HitPoints>& hit_points);
	~HitPointsGauge();

	void LateUpdate();
	void Draw() const;

private:
	static constexpr Vector2D<int>	kScreenSize			= { 256, 256 };
	static constexpr float			kMaxGaugePercent	= 75.0f;
	static constexpr int			kBaseGaugeColor		= 0x404040;
	static constexpr int			kThickness			= 20;

	std::shared_ptr<HitPoints>&		m_hit_points;
	std::shared_ptr<ScreenCreator>	m_circle_screen;
	std::shared_ptr<ScreenCreator>	m_current_hit_points_gauge_screen;
	std::shared_ptr<ScreenCreator>	m_gauge_screen;
	std::shared_ptr<Graphicer>		m_graphicer;

	float m_current_hit_points_gauge_percent;
	float m_current_max_hit_points_gauge_percent;
};
