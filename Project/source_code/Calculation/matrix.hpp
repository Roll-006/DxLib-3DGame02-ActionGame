#pragma once
#include <DxLib.h>

inline MATRIX operator+ (const MATRIX& m1, const MATRIX& m2)	{ return MAdd (m1, m2); }
inline MATRIX operator* (const MATRIX& m1, const MATRIX& m2)	{ return MMult(m1, m2); }

template<typename ScaleT>
inline MATRIX operator* (const MATRIX& m, const ScaleT scale)	{ return MScale(m, scale); }
template<typename ScaleT>
inline MATRIX operator* (const ScaleT scale, const MATRIX& m)	{ return MScale(m, scale); }

inline MATRIX operator+=(MATRIX& m1, const MATRIX& m2)			{ m1 = m1 + m2; return m1; }
inline MATRIX operator*=(MATRIX& m1, const MATRIX& m2)			{ m1 = m1 * m2; return m1; }

template<typename ScaleT>
inline MATRIX operator*=(MATRIX& m, const ScaleT scale)			{ m = m * scale; return m; }

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
