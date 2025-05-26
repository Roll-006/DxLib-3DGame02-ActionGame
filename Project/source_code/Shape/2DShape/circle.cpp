//#include "../../../Calculator/math.hpp"
//#include "circle.hpp"
//
//Circle::Circle(bool has_internal, const VECTOR& normal_vector, float radius):
//	ShapeBase(has_internal ? ShapeKind::kCircle : ShapeKind::kCircumference),
//	m_pos		(v3d::GetZeroVector()),
//	m_normal_vector	(normal_vector),
//	m_radius		(radius),
//	m_interval_andle(0.0f)
//{
//	m_interval_andle = 360.0f / kVertexNum * math::kDegreesToRadian;
//
//	Init(m_normal_vector, m_radius);
//
//	for (auto& triangle : m_triangle)
//	{
//		triangle = new Triangle;
//	}
//}
//
//Circle::Circle(bool has_internal) :
//	ShapeBase(has_internal ? ShapeKind::kCircle : ShapeKind::kCircumference),
//	m_pos		(v3d::GetZeroVector()),
//	m_normal_vector	(v3d::GetZeroVector()),
//	m_radius		(0.0f),
//	m_interval_andle(0.0f)
//{
//	m_interval_andle = 360.0f / kVertexNum * math::kDegreesToRadian;
//
//	Init(m_normal_vector, m_radius);
//
//	for (auto& triangle : m_triangle)
//	{
//		triangle = new Triangle;
//	}
//}
//
//Circle::~Circle()
//{
//	for (auto& triangle : m_triangle)
//	{
//		delete triangle;
//	}
//}
//
//void Circle::Init(const VECTOR& normal_vector, float radius)
//{
//	m_normal_vector = math::GetNormalizedVector(normal_vector);
//	m_radius        = radius;
//}
//
//void Circle::SetPos(const VECTOR& pos)
//{
//	VECTOR dir1 = math::GetNormalVector(m_normal_vector);
//	VECTOR dir2 = math::GetNormalVector(m_normal_vector, dir1);
//	m_pos		  = pos;
//	
//	for (size_t i = 0; i < m_triangle.size(); ++i)
//	{
//		VECTOR pos1 = m_pos + m_radius * (cosf(m_interval_andle * i      ) * dir1 + sinf(m_interval_andle * i      ) * dir2);
//		VECTOR pos2 = m_pos + m_radius * (cosf(m_interval_andle * (i + 1)) * dir1 + sinf(m_interval_andle * (i + 1)) * dir2);
//
//		m_triangle.at(i)->MakeTriangle(pos1, pos2, m_pos);
//	}
//}
//
//void Circle::Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num) const
//{
//	if (is_draw_normal_vector)
//	{
//		DrawLine3D(m_pos, m_pos + m_normal_vector * kNormalVectorLength, 0xd900ff);
//	}
//
//	if (is_draw_frame)
//	{
//		for (auto& triangle : m_triangle)
//		{
//			DrawLine3D(triangle->GetPos(0), triangle->GetPos(1), 0xff0000);
//		}
//	}
//
//	if (alpha_blend_num > 0)
//	{
//		for (auto& triangle : m_triangle)
//		{
//			triangle->Draw(false, false, alpha_blend_num);
//		}
//	}
//}
