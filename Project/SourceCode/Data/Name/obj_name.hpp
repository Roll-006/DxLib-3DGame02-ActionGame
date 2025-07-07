#pragma once
#include <string>

static struct ObjName
{
	const std::string MAIN_CAMERA		= "main_camera";		// メインカメラ
	const std::string TRACK_CAMERA		= "track_camera";		// 追跡カメラ
	const std::string AIMING_CAMERA		= "aiming_camera";		// 銃を構える際のカメラ
	const std::string SCOPE_CAMERA		= "scope_camera";		// 銃のスコープ視点のカメラ

	const std::string PLAYER			= "player";
	const std::string ZOMBIE_POLICE		= "zombie_police";

	const std::string HOUSE				= "house";
	const std::string GROUND			= "ground";

	const std::string SELECTOR			= "selector";

	const std::string ASSAULT_RIFLE		= "assault_rifle";

	const std::string BULLET			= "bullet";
	const std::string SHELL_CASING		= "shell_casing";

	const std::string NIGHT_SKY			= "night_sky";

}ObjName;
