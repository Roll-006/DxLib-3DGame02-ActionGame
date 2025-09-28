#pragma once

/// @brief ダウン中の敵がカメラの視界に入った
struct OnDownedEnemySpottedEvent
{
	int		target_obj_handle	= -1;
	float	camera_diff_angle	= 0.0f;	// 中心からの角度の差
};
