#pragma once
#include <array>
#include <unordered_map>
#include <DxLib.h>

#include "../Kind/time_kind.hpp"

struct ControlVirtualCameraData
{
	VECTOR									move_dir			= {};			// 移動方向
	VECTOR									velocity			= {};			// 速度ベクトル
	std::unordered_map<TimeKind, VECTOR>	input_angle			= {};			// 入力角度
	float									init_angle_speed	= 0.0f;			// 視点リセット速度
	float									init_end_threshold	= 0.0f;			// 視点リセットを終了する閾値
	bool									is_init_aiming		= false;		// 視点リセット中かを判定
};
