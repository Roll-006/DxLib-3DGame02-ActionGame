#include "status_icons.hpp"

StatusIcons::StatusIcons(const std::shared_ptr<Player>& player) : 
	m_screen_creator			(std::make_shared<ScreenCreator>(kScreenSize)),
	m_health_gauge				(std::make_shared<HealthGauge>(player->GetHealth(HealthPartKind::kMain))),
	m_equip_weapon_icon			(std::make_shared<EquipWeaponIcon>(player->GetCurrentEquipWeapons())),
	m_health_gauge_graphic		(std::make_shared<Graphicer>(m_health_gauge->GetScreenHandle())),
	m_equip_weapon_icon_graphic	(std::make_shared<Graphicer>(m_equip_weapon_icon->GetScreenHandle()))
{
	m_screen_creator->GetGraphicer()->SetCenterPos(Vector2D<int>(
		static_cast<int>(Window::kScreenSize.x * 0.891f), 
		static_cast<int>(Window::kScreenSize.y * 0.81f)));

	m_health_gauge_graphic		->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
	m_equip_weapon_icon_graphic	->SetCenterPos(Vector2D<int>(kScreenSize.x * 0.5f, kScreenSize.y * 0.5f));
}

StatusIcons::~StatusIcons()
{

}

void StatusIcons::LateUpdate()
{
	m_health_gauge		->LateUpdate();
	m_equip_weapon_icon	->LateUpdate();

	CreateScreen();
}

void StatusIcons::Draw() const
{
	m_screen_creator->Draw();
}

void StatusIcons::CreateScreen()
{
	m_screen_creator->UseScreen();

	m_health_gauge_graphic		->Draw();
	m_equip_weapon_icon_graphic	->Draw();

	m_screen_creator->UnuseScreen();
}
