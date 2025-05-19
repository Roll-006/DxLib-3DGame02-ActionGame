#include "segment.hpp"

Segment::Segment(const VECTOR& begin_pos, const VECTOR& end_pos):
	ShapeBase	(ShapeKind::kSegment),
	m_begin_pos	(begin_pos),
	m_end_pos	(end_pos),
	m_dir		(v3d::GetZeroVector()),
	m_length    (0.0f)
{
	m_dir		= v3d::GetNormalizedVector(m_end_pos - m_begin_pos);
	m_length	= VSize(m_dir);
}

Segment::Segment(const VECTOR& begin_pos, const VECTOR& dir, const float length) :
	ShapeBase	(ShapeKind::kSegment),
	m_begin_pos	(begin_pos),
	m_end_pos	(v3d::GetZeroVector()),
	m_dir		(v3d::GetNormalizedVector(dir)),
	m_length    (length)
{
	m_end_pos = m_begin_pos + m_dir * m_length;
}

Segment::Segment() :
	ShapeBase	(ShapeKind::kSegment),
	m_begin_pos	(v3d::GetZeroVector()),
	m_end_pos	(v3d::GetZeroVector()),
	m_dir		(v3d::GetZeroVector()),
	m_length	(0.0f)
{
	// èàóùÇ»Çµ
}

Segment::~Segment()
{
	// èàóùÇ»Çµ
}

void Segment::Draw(const unsigned int color)const
{
	DrawLine3D(m_begin_pos, m_end_pos, color);
}

void Segment::Move(const VECTOR& velocity)
{
	m_begin_pos += velocity;
	m_end_pos	= m_begin_pos + m_dir * m_length;
}

void Segment::SetPos(const VECTOR& begin_pos)
{
	m_begin_pos = begin_pos;
	m_end_pos   = m_begin_pos + m_dir * m_length;
}
