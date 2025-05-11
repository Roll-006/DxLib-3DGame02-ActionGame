#pragma once
#include <cmath>
#include "vector_2d.hpp"

namespace v2d
{
	template<typename CastT>
	inline Vector2D<CastT> ConvertVecType(const v2d_concepts::ValidT auto& v)
	{
		return Vector2D<CastT>(v.x, v.y);
	}

	inline float GetSize		(const v2d_concepts::ValidT auto& v)
	{
		return static_cast<float>(sqrt(v.x * v.x + v.y * v.y));
	}
	inline float GetSquareSize	(const v2d_concepts::ValidT auto& v)
	{
		return GetSize(v) * GetSize(v);
	}

	template<v2d_concepts::ValidT VecT>
	inline VecT GetZeroVector()
	{
		return VecT(0.0f, 0.0f);
	}

	template<v2d_concepts::ValidT VecT>
	inline VecT GetNormalizedVector(const VecT& v)
	{
		float size = GetSize(v);
		return size != 0 ? VecT(v.x / size, v.y / size) : v;
	}
}
