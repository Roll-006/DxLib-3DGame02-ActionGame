#pragma once
#include <DxLib.h>
#include "../Calculation/vector_3d.hpp"

/// @brief XYZŽ²
struct Axes
{
	VECTOR x_axis;
	VECTOR y_axis;
	VECTOR z_axis;
};

namespace axis
{
	[[nodiscard]] inline VECTOR GetWorldXAxis() { return VGet(1.0f, 0.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldYAxis() { return VGet(0.0f, 1.0f, 0.0f); }
	[[nodiscard]] inline VECTOR GetWorldZAxis() { return VGet(0.0f, 0.0f, 1.0f); }
	[[nodiscard]] inline Axes   GetWorldAxes () { return Axes(GetWorldXAxis(), GetWorldYAxis(), GetWorldZAxis()); }

	inline void Draw(const Axes& axes, const VECTOR& begin_pos, const float length)
	{
		DrawLine3D(begin_pos, begin_pos + axes.x_axis * length, GetColor(255,   0,   0));
		DrawLine3D(begin_pos, begin_pos + axes.y_axis * length, GetColor(  0, 255,   0));
		DrawLine3D(begin_pos, begin_pos + axes.z_axis * length, GetColor(  0,   0, 255));
	};
}