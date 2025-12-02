#pragma once
#include <DxLib.h>
#include "../Kind/axis_kind.hpp"

struct AxisData
{
	VECTOR		axis = {};
	AxisKind	kind = AxisKind::kRight;
};
