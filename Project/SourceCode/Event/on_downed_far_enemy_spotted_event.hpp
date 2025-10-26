#pragma once

/// @brief 遠距離のダウン中の敵がカメラの視界に入った
struct OnDownedFarEnemySpottedEvent
{
	int	  target_obj_handle		= -1;
	float camera_diff_angle		= 0.0f;		// カメラのforwardからの角度の差
	float distance_to_camera	= 0.0f;
};
