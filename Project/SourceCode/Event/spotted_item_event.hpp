#pragma once

/// @brief アイテムがカメラの視界に入った
struct SpottedItemEvent
{
	int	  target_obj_handle		= -1;
	float camera_diff_angle		= 0.0f;		// カメラのforwardからの角度の差
	float distance_to_camera	= 0.0f;
};
