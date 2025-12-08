#pragma once
#include "../Kind/item_kind.hpp"
#include "../Kind/time_scale_layer_kind.hpp"

/// @brief ƒAƒCƒeƒ€‚ðŽæ“¾‚µ‚½
struct PickUpItemEvent
{
	ItemKind			item_kind				= ItemKind::kNone;
	int					obj_handle				= -1;
	VECTOR				owner_pos				= {};
	TimeScaleLayerKind	time_scale_layer_kind	= TimeScaleLayerKind::kNoneScale;
};
