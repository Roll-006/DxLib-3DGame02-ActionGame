#include "plane.hpp"

Plane::Plane(const VECTOR& pos, const VECTOR& normal_vector):
	ShapeBase		(ShapeKind::kPlane),
	m_pos			(pos),
	m_normal_vector	(v3d::GetNormalizedV(normal_vector))
{
	// 処理なし
}

Plane::Plane() :
	ShapeBase		(ShapeKind::kPlane),
	m_pos			(v3d::GetZeroV()),
	m_normal_vector	(v3d::GetZeroV())
{
	// 処理なし
}

Plane::~Plane()
{
	// 処理なし
}

void Plane::Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const
{
	CreateDrawSquare(kDrawDdgeLength).Draw(is_draw_frame, alpha_blend_num, frame_color);
}

Square&& Plane::CreateDrawSquare(const float edge_length) const
{
	const auto dir1 = math::GetNormalVector(m_normal_vector);
	const auto dir2 = math::GetNormalVector(m_normal_vector, dir1);

	auto vertex1 = m_pos + dir1 * edge_length * 0.5f;
	vertex1	+= dir2 * edge_length * 0.5f;
	const auto vertex2 = vertex1 - dir1 * edge_length;
	const auto vertex3 = vertex2 - dir2 * edge_length;
	const auto vertex4 = vertex3 + dir1 * edge_length;

	return Square(vertex1, vertex2, vertex3, vertex4);
}

void Plane::Move(const VECTOR& velocity)
{
	m_pos += velocity;
}
