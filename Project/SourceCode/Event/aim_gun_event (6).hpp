#pragma once
#include <DxLib.h>
#include "../Kind/time_scale_layer_kind.hpp"

struct AimGunEvent
{
	VECTOR				pos						= {};
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};
