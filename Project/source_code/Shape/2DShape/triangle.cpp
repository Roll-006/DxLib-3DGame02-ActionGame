#include "triangle.hpp"

Triangle::Triangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3):
	ShapeBase(ShapeKind::kTriangle),
    m_normal_vector	(v3d::GetZeroVector()),
	m_centroid		(v3d::GetZeroVector()),
	m_image_handle	(-1)
{
	for (auto& edge : m_edge)
	{
		edge = Segment();
	}

    Init(pos1, pos2, pos3);

	for (int i = 0; i < kVertexNum; ++i)
	{
		m_vertex[i].norm	= VGet(0.0f, 1.0f, 0.0f);
		m_vertex[i].dif		= GetColorU8(255, 255, 255, 255);
		m_vertex[i].spc		= GetColorU8(255, 255, 255, 255);
		m_vertex[i].u		= 0.0f;
		m_vertex[i].v		= 0.0f;
		m_vertex[i].su		= 0.0f;
		m_vertex[i].sv		= 0.0f;
	}
}

Triangle::Triangle() :
	ShapeBase(ShapeKind::kTriangle),
	m_normal_vector	(v3d::GetZeroVector()),
	m_centroid		(v3d::GetZeroVector()),
	m_image_handle	(-1)
{
	for (auto& edge : m_edge)
	{
		edge = Segment();
	}

	Init(v3d::GetZeroVector(), v3d::GetZeroVector(), v3d::GetZeroVector());

	for (int i = 0; i < kVertexNum; ++i)
	{
		m_vertex[i].norm	= VGet(0.0f, 1.0f, 0.0f);
		m_vertex[i].dif		= GetColorU8(255, 255, 255, 255);
		m_vertex[i].spc		= GetColorU8(255, 255, 255, 255);
		m_vertex[i].u		= 0.0f;
		m_vertex[i].v		= 0.0f;
		m_vertex[i].su		= 0.0f;
		m_vertex[i].sv		= 0.0f;
	}
}

Triangle::~Triangle()
{
	// ˆ—‚È‚µ
}

void Triangle::Init(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3)
{
	m_vertex[0].pos = pos1;
	m_vertex[1].pos = pos2;
	m_vertex[2].pos = pos3;
	m_edge.at(0).MakeSegment(pos1, pos2);
	m_edge.at(1).MakeSegment(pos2, pos3);
	m_edge.at(2).MakeSegment(pos3, pos1);
	m_normal_vector = v3d::GetNormalVector(m_vertex[1].pos - m_vertex[2].pos, m_vertex[0].pos - m_vertex[2].pos);
	m_centroid		= math::GetCentroidOfATriangle(m_vertex[0].pos, m_vertex[1].pos, m_vertex[2].pos);
}

void Triangle::Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num)const
{
	if (is_draw_normal_vector)
	{
		DrawLine3D(m_centroid, m_centroid + m_normal_vector * kNormalVectorLength, 0xd900ff);
	}

	if (is_draw_frame)
	{
		for (auto& edge : m_edge)
		{
			edge->Draw();
		}
	}

	if (alpha_blend_num > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend_num);
		if (m_image_handle == -1)
		{
			DrawPolygon3D(m_vertex, 1, DX_NONE_GRAPH, TRUE);
		}
		else
		{
			DrawPolygon3D(m_vertex, 1, m_image_handle, TRUE);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Triangle::LoadTexture(std::string image_name, float u1, float v1, float u2, float v2, float u3, float v3)
{
	m_image_handle = LoadGraph(image_name.c_str(), TRUE);

	m_vertex[0].u = u1;
	m_vertex[0].v = v1;
	m_vertex[1].u = u2;
	m_vertex[1].v = v2;
	m_vertex[2].u = u3;
	m_vertex[2].v = v3;
}