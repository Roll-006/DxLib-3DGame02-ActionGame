#pragma once
#include <string>

static struct UIGraphicPath
{
	#pragma region 武器
	const std::string ASSAULT_RIFLE			= "Data/Graphic/UI/Weapon/assault_rifle.png";
	const std::string ROCKET_LAUNCHER		= "Data/Graphic/UI/Weapon/rocket_launcher.png";
	const std::string KNIFE					= "Data/Graphic/UI/Weapon/knife.png";
	const std::string GRENADE				= "Data/Graphic/UI/Weapon/grenade.png";
	#pragma endregion


	#pragma region HPゲージ
	const std::string CURRENT_HEALTH_GAUGE	= "Data/Graphic/UI/HealthGauge/current_health.png";
	const std::string HEALTH_GAUGE_PARTICLE	= "Data/Graphic/UI/HealthGauge/particle.png";
	const std::string DAMAGE_GAUGE_PARTICLE	= "Data/Graphic/UI/HealthGauge/damage_particle.png";
	const std::string GAUGE_FRAME_PARTICLE	= "Data/Graphic/UI/HealthGauge/frame_particle.png";
	const std::string WARNING_CIRCLE		= "Data/Graphic/UI/HealthGauge/warning.png";
	#pragma endregion


	const std::string DAMAGE_FILTER			= "Data/Graphic/UI/Damage/filter.png";


	#pragma region ボタン
	const std::string KEY_F					= "Data/Graphic/UI/Button/Key/F.png";
	const std::string PAD_A					= "Data/Graphic/UI/Button/Pad/A.png";
	#pragma endregion

}UIGraphicPath;
