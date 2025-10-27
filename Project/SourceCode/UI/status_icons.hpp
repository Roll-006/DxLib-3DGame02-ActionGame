#pragma once
#include "../Object/player.hpp"
#include "health_gauge.hpp"
#include "equip_weapon_icon.hpp"
#include "../Event/event_system.hpp"

class StatusIcons final
{
public:
	StatusIcons(const std::shared_ptr<Player>& player);
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
	static constexpr Vector2D<int>	kScreenSize = { 500, 500 };

	std::shared_ptr<ScreenCreator>		m_result_screen;
	std::shared_ptr<HealthGauge>		m_health_gauge;
	std::shared_ptr<EquipWeaponIcon>	m_equip_weapon_icon;
	std::shared_ptr<Graphicer>			m_health_gauge_graphic;
	std::shared_ptr<Graphicer>			m_equip_weapon_icon_graphic;

	int	  m_alpha_blend_num;
	bool  m_is_active_cutscene;
};
