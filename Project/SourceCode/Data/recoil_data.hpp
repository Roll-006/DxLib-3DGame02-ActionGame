#pragma once
#include <DxLib.h>

struct RecoilData
{
	float  max_yaw			= 0.0f;					// 指定可能範囲 :  0～1
	float  min_yaw			= 0.0f;					// 指定可能範囲 : -1～0
	float  yaw				= 0.0f;					// ヨー(Y軸)回転値 正の値で指定
	float  pitch			= 0.0f;					// ピッチ(X軸)回転値 正の値で指定
	float  recoil_time		= 0.0f;					// 最高地点に到達するまでの時間
	VECTOR return_damping	= { 1.0f, 1.0f, 1.0f };	// 元地点への復帰する際の減衰値
};
