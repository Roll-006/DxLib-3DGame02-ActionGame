#include "../../../Calculator/math.hpp"

#include "plane.hpp"

Plane::Plane(const VECTOR& normal_vec, float draw_edge_length):
	ShapeBase	(ShapeKind::kPlane),
	m_pos				(math::GetZeroVector()),
	m_normal_vector		(normal_vec),
	m_draw_edge_length	(draw_edge_length)
{
	m_square = new Square;
	Init(m_normal_vector, m_draw_edge_length);
}

Plane::Plane() :
	ShapeBase	(ShapeKind::kPlane),
	m_pos			(math::GetZeroVector()),
	m_normal_vector		(math::GetZeroVector()),
	m_draw_edge_length	(0.0f)
{
	m_square = new Square;
	Init(m_normal_vector, m_draw_edge_length);
}

Plane::~Plane()
{

}

void Plane::Init(const VECTOR& normal_vector, float draw_edge_length)
{
	m_normal_vector		= math::GetNormalizedVector(normal_vector);
	m_draw_edge_length	= draw_edge_length;
}

void Plane::Update(const VECTOR& pos)
{
	m_pos = pos;

	VECTOR dir1 = math::GetNormalVector(m_normal_vector);
	VECTOR dir2 = math::GetNormalVector(m_normal_vector, dir1);

	VECTOR vertex1 = m_pos + dir1 * m_draw_edge_length * 0.5f;
	vertex1 += dir2 * m_draw_edge_length * 0.5f;
	VECTOR vertex2 = vertex1 - dir1 * m_draw_edge_length;
	VECTOR vertex3 = vertex2 - dir2 * m_draw_edge_length;
	VECTOR vertex4 = vertex3 + dir1 * m_draw_edge_length;

	m_square->Init(vertex1, vertex2, vertex3, vertex4);
}

void Plane::Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num, float draw_edge_length)const
{
	m_square->Draw(is_draw_normal_vector, is_draw_frame, alpha_blend_num);
}
