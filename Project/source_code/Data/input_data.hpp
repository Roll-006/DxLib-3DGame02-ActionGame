#pragma once
#include <unordered_map>
#include "../Data/IncludeList/vector.hpp"

struct InputData
{
	bool  is_input;
	float input_time;
};

struct MouseData
{
	Vector2D<int>   pos;
	Vector2D<float> dir;
	Vector2D<float> velocity;
	int wheel_rotation;
};
