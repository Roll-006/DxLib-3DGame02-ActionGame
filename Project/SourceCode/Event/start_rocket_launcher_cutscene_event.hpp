#pragma once
#include "../Kind/gun_kind.hpp"
#include "../Part/transform.hpp"

/// @brief ロケットランチャーが弾丸を発射した(必殺技)
struct StartRocketLauncherCutsceneEvent
{
	float world_time_scale	= 1.0f;
	float player_time_scale = 1.0f;
	float effect_time_scale = 1.0f;
};
