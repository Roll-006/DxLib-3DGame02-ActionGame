#pragma once
#include <DxLib.h>

/// @brief XYZŽ²
struct Axes
{
	VECTOR x_axis;
	VECTOR y_axis;
	VECTOR z;
};

namespace axis
{
	[[nodiscard]] inline VECTOR GetWorldXAxis() { return VGet(1.0f, 0.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldYAxis() { return VGet(0.0f, 1.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldZAxis() { return VGet(0.0f, 0.0f, 1.0f); }
	[[nodiscard]] inline Axes   GetWorldAxes () { return Axes(GetWorldXAxis(), GetWorldYAxis(), GetWorldZAxis()); }
}