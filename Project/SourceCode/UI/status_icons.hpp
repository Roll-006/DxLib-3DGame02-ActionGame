#pragma once
#include "../Object/player.hpp"
#include "health_gauge.hpp"

class StatusIcons final
{
public:
	StatusIcons(const std::shared_ptr<Player>& player);
	~StatusIcons();

	void LateUpdate();
	void Draw() const;

private:
	void CreateScreen();

private:
	static constexpr Vector2D<int>	kScreenSize = { 300, 300 };

	std::shared_ptr<ScreenCreator>	m_screen_creator;
	std::shared_ptr<HealthGauge>	m_health_gauge;
	std::shared_ptr<Graphicer>		m_health_gauge_graphic;
};
