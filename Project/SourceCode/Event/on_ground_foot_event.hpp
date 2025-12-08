#pragma once
#include <DxLib.h>
#include "../Kind/time_scale_layer_kind.hpp"

/// @brief ‘«‚ª’n–Ê‚É’…’n‚µ‚½
struct OnGroundFootEvent
{
	bool				is_run					= false;
	bool				is_left_foot			= false;
	VECTOR				pos						= {};
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};
