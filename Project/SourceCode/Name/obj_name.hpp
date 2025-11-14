#pragma once
#include <string>

static struct ObjName
{
	#pragma region カメラ
	const std::string MAIN_CAMERA								= "main_camera";
	const std::string TITLE_CAMERA								= "title_camera";
	const std::string ROT_CONTROL_VIRTUAL_CAMERA				= "rot_control_virtual_camera";
	const std::string AIM_CONTROL_VIRTUAL_CAMERA				= "aim_control_virtual_camera";
	const std::string DEAD_PLAYER_VIRTUAL_CAMERA				= "dead_player_virtual_camera";
	const std::string DEAD_BOSS_VIRTUAL_CAMERA					= "dead_boss_virtual_camera";
	const std::string VICTORY_POSE_VIRTUAL_CAMERA				= "victory_pose_virtual_camera";
	const std::string GRAB_VIRTUAL_CAMERA						= "grab_virtual_camera";
	const std::string STEALTH_KILL_VIRTUAL_CAMERA				= "stealth_kill_virtual_camera";
	const std::string ROCKET_LAUNCHER_ENTER_ROT_VIRTUAL_CAMERA	= "rocket_launcher_enter_rot_virtual_camera";
	const std::string ROCKET_LAUNCHER_EXIT_ROT_VIRTUAL_CAMERA	= "rocket_launcher_exit_rot_virtual_camera";
	const std::string ROCKET_LAUNCHER_ZOOM_IN_VIRTUAL_CAMERA	= "rocket_launcher_zoom_in_virtual_camera";
	const std::string ROCKET_LAUNCHER_ZOOM_OUT_VIRTUAL_CAMERA	= "rocket_launcher_zoom_out_virtual_camera";
	#pragma endregion


	#pragma region キャラクター
	const std::string PLAYER									= "player";
	const std::string ZOMBIE									= "zombie";
	const std::string BOSS										= "boss";
	const std::string TITLE_CHARACTER							= "title_character";
	#pragma endregion


	#pragma region 建物
	const std::string HOUSE										= "house";
	const std::string HOUSES									= "houses";
	const std::string GROUND									= "ground";
	const std::string TREES										= "trees";
	#pragma endregion


	#pragma region 武器
	const std::string KNIFE										= "knife";
	const std::string ASSAULT_RIFLE								= "assault_rifle";
	const std::string ROCKET_LAUNCHER							= "rocket_launcher";
	#pragma endregion


	#pragma region 弾丸
	const std::string BULLET									= "bullet";
	const std::string ROCKET_BOMB								= "rocket_bomb";
	const std::string SHELL_CASING_556x45						= "shell_casing_556x45";
	const std::string ASSAULT_RIFLE_MAGAZINE					= "assault_rifle_magazine";
	const std::string AMMO_BOX_556x45							= "ammo_box_556x45";
	const std::string AMMO_BOX_ROCKET_BOMB						= "ammo_box_rocket_bomb";
	const std::string NON_COLLIDE_ROCKET_BOMB					= "non_collide_rocket_bomb";
	#pragma endregion


	#pragma region スカイドーム
	const std::string NIGHT_SKY									= "night_sky";
	#pragma endregion


	#pragma region エフェクト
	const std::string TITLE_SMOKE								= "title_smoke";
	const std::string LIGHT_ROCKET_LAUNCHER_EXHAUST_VENT_EFFECT = "light_rocket_launcher_exhaust_vent_effect";
	const std::string EXPANDING_SMOKE_EFFECT					= "expanding_smoke_effect";
	const std::string ROCKET_BOMB_HIT_EXPLOSION_EFFECT			= "rocket_bomb_hit_explosion_effect";
	const std::string HIT_BULLET_LIGHT_EFFECT					= "hit_bullet_light_effect";
	const std::string ROCKET_BOMB_SMOKE_EFFECT					= "rocket_bomb_smoke_effect";
	const std::string DISAPPEAR_SMOKE_EFFECT					= "disappear_smoke_effect";
	const std::string SHOT_FIRE_EFFECT							= "shot_fire_effect";
	const std::string BLOOD_EFFECT								= "blood_effect";
	const std::string ITEM_RED_EFFECT							= "item_red_effect";
	const std::string ITEM_GREEN_EFFECT							= "item_green_effect";
	#pragma endregion


	#pragma region その他
	const std::string SELECTOR									= "selector";
	#pragma endregion

}ObjName;
