#pragma once
#include <DxLib.h>

/// @brief ターゲットを発見した
struct OnTargetDetectedEvent
{
	int		enemy_handle	= -1;		// 発見した敵のハンドル
	float	notify_distance = 0.0f;		// 周囲の敵に通知を送る距離
	VECTOR  notify_pos		= {};		// 通知が送られる座標
};
