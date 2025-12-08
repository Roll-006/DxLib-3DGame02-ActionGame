#pragma once
#include <string>
#include <DxLib.h>
#include "../Kind/time_scale_layer_kind.hpp"

/// @brief íeä€Ç™è’ìÀÇµÇΩ
struct OnHitBulletEvent
{
	std::string			bullet_name				= "";
	VECTOR				hit_pos					= {};
	VECTOR				move_dir				= {};
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};
