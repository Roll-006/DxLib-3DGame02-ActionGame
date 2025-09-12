#include "status_icons.hpp"

StatusIcons::StatusIcons(const std::shared_ptr<Player> player) : 
	m_hit_points_gauge(std::make_shared<HitPointsGauge>(player->GetHitPoints(HitPointsPartKind::kMain)))
{

}

StatusIcons::~StatusIcons()
{

}

void StatusIcons::LateUpdate()
{
	m_hit_points_gauge->LateUpdate();
}

void StatusIcons::Draw() const
{
	m_hit_points_gauge->Draw();
}
