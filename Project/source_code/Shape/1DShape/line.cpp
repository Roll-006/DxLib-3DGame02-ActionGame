#include "line.hpp"

Line::Line(const VECTOR& pos, const VECTOR& dir):
	ShapeBase(ShapeKind::kLine),
	m_pos(pos),
	m_dir(v3d::GetNormalizedVector(dir))
{
	// èàóùÇ»Çµ
}

Line::Line() :
	ShapeBase(ShapeKind::kLine),
	m_pos(v3d::GetZeroVector()),
	m_dir(v3d::GetZeroVector())
{
	// èàóùÇ»Çµ
}

Line::~Line()
{

}

void Line::Draw(const int draw_length, const unsigned int color)
{
	VECTOR begin = m_pos - m_dir * draw_length * 0.5f;
	VECTOR end = begin + m_dir * draw_length;
	DrawLine3D(begin, end, color);
}

void Line::Move(const VECTOR& velocity)
{
	m_pos += velocity;
}

void Line::SetPos(const VECTOR& pos)
{
	m_pos = pos;
}
