#pragma once
#include <unordered_map>
#include "../Data/IncludeList/vector.hpp"
#include "../Data/Kind/input_kind.hpp"

struct InputCode
{
	InputKind kind;
	int code;

	InputCode(){}
	InputCode(const InputKind kind, const int code) : kind(kind), code(code){}
};

struct InputData
{
	bool  is_input;
	float input_time;

	InputData() : is_input(false), input_time(0.0f){}
	InputData(const bool is_input, const float input_time) : is_input(is_input), input_time(input_time) {}
};

struct MouseData
{
	Vector2D<int>   pos;
	Vector2D<float> dir;
	Vector2D<float> velocity;
	int wheel_rotation;

	MouseData() : 
		pos				(v2d::GetZeroVector<Vector2D<int>>()), 
		dir				(v2d::GetZeroVector<Vector2D<float>>()), 
		velocity		(v2d::GetZeroVector<Vector2D<float>>()), 
		wheel_rotation	(0)
	{ }

	MouseData(const Vector2D<int> pos, const Vector2D<float> dir, const Vector2D<float> velocity, const int wheel_rotation) :
		pos				(pos),
		dir				(dir),
		velocity		(velocity),
		wheel_rotation	(wheel_rotation)
	{ }
};
