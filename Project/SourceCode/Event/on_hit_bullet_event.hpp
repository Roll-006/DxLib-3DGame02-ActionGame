#pragma once
#include <string>
#include <DxLib.h>

/// @brief íeä€Ç™è’ìÀÇµÇΩ
struct OnHitBulletEvent
{
	std::string	bullet_name	= "";
	VECTOR		hit_pos		= {};
	VECTOR		move_dir	= {};
};
