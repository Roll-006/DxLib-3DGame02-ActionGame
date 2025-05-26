#pragma once
#include <DxLib.h>

namespace matrix
{
	inline void Draw(const MATRIX& mat, const VECTOR& first_draw_pos)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				int pos_x = static_cast<int>(first_draw_pos.x + j * 200);
				int pos_y = static_cast<int>(first_draw_pos.y + i * 20);
				DrawFormatString(pos_x, pos_y, 0xffffff, "%f", mat.m[i][j]);
			}
		}
	}
}