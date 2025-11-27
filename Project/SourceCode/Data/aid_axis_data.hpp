#pragma once
#include <DxLib.h>
#include "../Kind/axis_kind.hpp"

struct AidAxisData
{
	VECTOR		axis = {};
	AxisKind	kind = AxisKind::kLeft;
};
