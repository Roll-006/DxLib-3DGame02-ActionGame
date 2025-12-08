#pragma once
#include "../Font/font_handler.hpp"
#include "../Calculation/vector_2d.hpp"

struct TextData
{
	int				font_handle = -1;
	std::string		text		= "";
	Vector2D<int>	font_size	= {};
};
