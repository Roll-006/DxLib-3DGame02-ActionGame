#pragma once

/// @brief ダウン中の敵がカメラの視界に入った
struct OnDownedEnemySpottedEvent
{
	int	  target_obj_handle		= -1;
	float camera_diff_angle		= 0.0f;		// カメラのforwardからの角度の差
	float distance_to_camera	= 0.0f;
};
