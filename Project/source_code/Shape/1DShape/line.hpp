#pragma once
#include <DxLib.h>
#include "../../Base/shape_base.hpp"

class Line : public ShapeBase
{
public:
	Line(const VECTOR& pos, const VECTOR& dir);
	Line();
	~Line();

	void MakeLine(const VECTOR& pos, const VECTOR& dir);

	/// @brief •`‰æ
	/// @param draw_length •`‰æ‚·‚éÛ‚Ì’·‚³
	void Draw(int color, int draw_length);

	void Move(const VECTOR& velocity);

	void SetPos(const VECTOR& pos);

	/// @brief ’¼üã‚Ì“_‚ğæ“¾
	/// @param length n“_‚©‚ç‚Ì’·‚³
	[[nodiscard]] VECTOR GetPoint(float length)const{ return m_pos + m_dir * length; }

	[[nodiscard]] VECTOR GetPos()const	{ return m_pos; }
	[[nodiscard]] VECTOR GetDir()const	{ return m_dir; }

private:
	VECTOR m_pos;
	VECTOR m_dir;
};
