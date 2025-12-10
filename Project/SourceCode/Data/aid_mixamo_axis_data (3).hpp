#pragma once
#include <DxLib.h>
#include "../Kind/mixamo_axis_kind.hpp"

struct AidMixamoAxisData
{
	VECTOR			axis = {};
	MixamoAxisKind	kind = MixamoAxisKind::kLeft;
};
