#pragma once
#include <string>
#include <DxLib.h>

/// @brief íeä€Ç™è’ìÀÇµÇΩ
struct OnHitBulletData
{
	std::string	bullet_name	= "";
	VECTOR		hit_pos		= {};
	VECTOR		move_dir	= {};
};
