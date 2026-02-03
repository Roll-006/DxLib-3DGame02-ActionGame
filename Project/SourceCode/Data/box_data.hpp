#pragma once
#include <array>

#include "../Shape/2DShape/square.hpp"

struct BoxData
{
	static constexpr int kVertexNum			= 8;
	static constexpr int kEdgeNum			= 12;
	static constexpr int kSquareNum			= 6;

	std::array<VECTOR, kVertexNum> vertexes	= {};	// 各頂点
	std::array<Square, kSquareNum> squares	= {};	// 各面
};
