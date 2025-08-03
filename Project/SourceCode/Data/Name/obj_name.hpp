#pragma once
#include <string>

static struct ObjName
{
	#pragma region カメラ
	const std::string MAIN_CAMERA			= "main_camera";				// メインカメラ
	const std::string ROT_CONTROL_CAMERA	= "rot_control_virtual_camera";	// 回転コントロールカメラ
	const std::string TRACK_CAMERA			= "track_camera";				// 追跡カメラ
	const std::string AIMING_CAMERA			= "aiming_camera";				// 銃を構える際のカメラ
	const std::string SCOPE_CAMERA			= "scope_camera";				// 銃のスコープ視点のカメラ
	#pragma endregion


	#pragma region キャラクター
	const std::string PLAYER				= "player";
	const std::string ZOMBIE_POLICE			= "zombie_police";
	#pragma endregion


	#pragma region 建物
	const std::string HOUSE					= "house";
	const std::string GROUND				= "ground";
	#pragma endregion


	#pragma region 武器
	const std::string ASSAULT_RIFLE			= "assault_rifle";
	const std::string KNIFE					= "knife";
	#pragma endregion


	#pragma region 弾丸
	const std::string BULLET				= "bullet";
	const std::string SHELL_CASING			= "shell_casing";
	#pragma endregion


	#pragma region スカイドーム
	const std::string NIGHT_SKY				= "night_sky";
	#pragma endregion


	#pragma region エフェクト
	const std::string TEST			= "test";
	#pragma endregion


	#pragma region その他
	const std::string SELECTOR				= "selector";
	#pragma endregion

}ObjName;
