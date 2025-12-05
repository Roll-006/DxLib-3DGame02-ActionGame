#pragma once
#include <DxLib.h>
#include "../Kind/time_scale_layer_kind.hpp"

/// @brief ダメージを受けた
struct OnDamageEvent
{
	VECTOR				hit_pos					= {};
	float				damage_parcent			= 0.0f;		// 最大HPに対するダメージの割合(damage / max_health)
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};
