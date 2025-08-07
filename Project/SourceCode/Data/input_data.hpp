#pragma once
#include <unordered_map>
#include "../Data/IncludeList/vector.hpp"
#include "../Data/Kind/input_kind.hpp"

struct InputCode
{
	InputKind	kind = InputKind::kKey;
	int			code = 0;
};

struct InputData
{
	bool  is_input		= false;
	float input_time	= 0.0f;
};

struct MouseData
{
	Vector2D<int>   pos				= v2d::GetZeroV<Vector2D<int>>();
	Vector2D<float> dir				= v2d::GetZeroV<Vector2D<float>>();
	Vector2D<float> velocity		= v2d::GetZeroV<Vector2D<float>>();
	int				wheel_rotation	= 0;
};
