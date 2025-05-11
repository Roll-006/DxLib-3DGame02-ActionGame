#pragma once
#include <cmath>
#include "vector_3d.hpp"

namespace v3d
{
	[[nodiscard]] VECTOR GetZeroVector();
	[[nodiscard]] VECTOR GetNormalizedVector(const VECTOR& v);
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& v);
	[[nodiscard]] VECTOR GetNormalVector(const VECTOR& v1, const VECTOR& v2);

	/// @brief ヨー角を取得
	[[nodiscard]] float GetYaw(const VECTOR& vector);
	/// @brief ヨー角回転ベクトルを取得
	[[nodiscard]] VECTOR GetYawRotateVector(const VECTOR& vector);
}