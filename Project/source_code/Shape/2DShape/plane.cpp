#include "plane.hpp"

Plane::Plane(const VECTOR& pos, const VECTOR& normal_vector):
	ShapeBase		(ShapeKind::kPlane),
	m_pos			(pos),
	m_normal_vector	(v3d::GetNormalizedVector(normal_vector))
{
	// èàóùÇ»Çµ
}

Plane::Plane() :
	ShapeBase		(ShapeKind::kPlane),
	m_pos			(v3d::GetZeroVector()),
	m_normal_vector	(v3d::GetZeroVector())
{
	// èàóùÇ»Çµ
}

Plane::~Plane()
{
	// èàóùÇ»Çµ
}

void Plane::Draw(bool is_draw_normal_vector, bool is_draw_frame, float draw_edge_length, int alpha_blend_num, unsigned int frame_color)
{
	MakeDrawSquare(draw_edge_length).Draw(is_draw_normal_vector, is_draw_frame, alpha_blend_num, frame_color);
}

Square Plane::MakeDrawSquare(float edge_length)
{
	VECTOR dir1 = math::GetNormalVector(m_normal_vector);
	VECTOR dir2 = math::GetNormalVector(m_normal_vector, dir1);

	VECTOR vertex1 = m_pos + dir1 * edge_length * 0.5f;
	vertex1 += dir2 * edge_length * 0.5f;
	VECTOR vertex2 = vertex1 - dir1 * edge_length;
	VECTOR vertex3 = vertex2 - dir2 * edge_length;
	VECTOR vertex4 = vertex3 + dir1 * edge_length;

	return Square(vertex1, vertex2, vertex3, vertex4);
}

void Plane::Move(const VECTOR& velocity)
{
	m_pos += velocity;
}
