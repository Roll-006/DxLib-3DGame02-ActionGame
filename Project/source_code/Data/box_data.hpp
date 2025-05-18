#pragma once
#include <array>

#include "../Shape/2DShape/square.hpp"

struct BoxData
{
	static constexpr int kEdgeNum = 12;

	std::array<VECTOR, 8> vertexes;	// Še’¸“_
	std::array<Square, 6> squares;	// Še–Ê
};
