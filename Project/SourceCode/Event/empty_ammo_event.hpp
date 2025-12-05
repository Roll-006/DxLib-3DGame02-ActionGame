#pragma once
#include <DxLib.h>
#include "../Kind/time_scale_layer_kind.hpp"

/// @brief Žc’e‚ª‚È‚©‚Á‚½
struct EmptyAmmoEvent
{
	VECTOR				pos						= {};
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};
