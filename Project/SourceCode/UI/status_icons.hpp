#pragma once
#include "../Object/player.hpp"
#include "health_gauge.hpp"
#include "equip_weapon_icon.hpp"
#include "remaining_bullets_ui.hpp"
#include "../Event/event_system.hpp"

class StatusIcons final
{
public:
	StatusIcons(std::shared_ptr<Player>& player);
	~StatusIcons();

	void LateUpdate();
	void Draw() const;

private:
	#pragma region Event
	void ActivateCutscene	(const StartRocketLauncherCutsceneEvent& event);
	void DeactivateCutscene	(const EndRocketLauncherCutsceneEvent&	 event);
	#pragma endregion


	void CreateScreen();
	void CalcResultScreenAlphaBlendNum();

private:
	static constexpr Vector2D<int> kScreenSize	= { 500, 500 };
	static constexpr Vector2D<int> kCenterPos	= { static_cast<int>(kScreenSize.x * 0.5f), static_cast<int>(kScreenSize.y * 0.5f) };

	std::shared_ptr<ScreenCreator>		m_result_screen;

	std::shared_ptr<HealthGauge>		m_health_gauge;
	std::shared_ptr<EquipWeaponIcon>	m_equip_weapon_icon;
	std::shared_ptr<RemainingBulletsUI> m_remaining_bullets_ui;

	std::shared_ptr<Graphicer>			m_health_gauge_graphic;
	std::shared_ptr<Graphicer>			m_equip_weapon_icon_graphic;
	std::shared_ptr<Graphicer>			m_remaining_bullets_ui_graphic;

	int	  m_alpha_blend_num;
	bool  m_is_active_cutscene;
};
