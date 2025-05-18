#include "../../../Calculator/math.hpp"
#include "square.hpp"

Square::Square(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4):
	ShapeBase(ShapeKind::kSquare),
	m_centroid		(math::GetZeroVector()),
	m_scale			(1.0f)
{
	m_triangle.at(0) = new Triangle(pos1, pos2, pos3);
	m_triangle.at(1) = new Triangle(pos1, pos3, pos4);
	Init(pos1, pos2, pos3, pos4);
}

Square::Square() :
	ShapeBase(ShapeKind::kSquare),
	m_centroid		(math::GetZeroVector()),
	m_scale			(1.0f)
{
	for (auto& triangle : m_triangle)
	{
		triangle = new Triangle;
	}
}

Square::~Square()
{
	for (auto& triangle : m_triangle)
	{
		delete triangle;
	}
}

void Square::Init(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4)
{
	m_triangle.at(0)->Init(pos1, pos2, pos3);
	m_triangle.at(1)->Init(pos1, pos3, pos4);
	m_centroid = math::GetCentroidOfAQuadrilateral(pos1, pos2, pos3, pos4);
}

void Square::Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num)const
{
	if (is_draw_normal_vector)
	{
		DrawLine3D(m_centroid, m_centroid + GetNormalVector() * kNormalVectorLength, 0xd900ff);
	}

	if (is_draw_frame)
	{
		m_triangle.at(0)->GetEdge(0)->Draw();
		m_triangle.at(0)->GetEdge(1)->Draw();
		m_triangle.at(1)->GetEdge(1)->Draw();
		m_triangle.at(1)->GetEdge(2)->Draw();
	}

	for (auto& triangle : m_triangle)
	{
		triangle->Draw(false, false, alpha_blend_num);
	}
}

void Square::LoadTexture(std::string image_name, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4)
{
	m_triangle.at(0)->LoadTexture(image_name, u1, v1, u2, v2, u3, v3);
	m_triangle.at(1)->LoadTexture(image_name, u1, v1, u3, v3, u4, v4);
}

void Square::LoadTexture(std::string image_name, TextureData::PasteDir texture_dir)
{
	// ³–Ê‚ÉŒü‚©‚Á‚Ä’£‚è•t‚¯‚éê‡‚ğ‰Šú’l‚Æ‚·‚é
	std::array<float, 8> texture_pos{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	switch (texture_dir)
	{
	case TextureData::PasteDir::kBack:
		std::rotate(texture_pos.begin(), next(texture_pos.begin(), 4), texture_pos.end());
		break;

	case TextureData::PasteDir::kLeft:
		std::rotate(texture_pos.begin(), next(texture_pos.begin(), 6), texture_pos.end());
		break;

	case TextureData::PasteDir::kRight:
		std::rotate(texture_pos.begin(), next(texture_pos.begin(), 2), texture_pos.end());
		break;

	default:
		break;
	}

	m_triangle.at(0)->LoadTexture(image_name, texture_pos.at(0), texture_pos.at(1), texture_pos.at(2), texture_pos.at(3), texture_pos.at(4), texture_pos.at(5));
	m_triangle.at(1)->LoadTexture(image_name, texture_pos.at(0), texture_pos.at(1), texture_pos.at(4), texture_pos.at(5), texture_pos.at(6), texture_pos.at(7));
}

VECTOR Square::GetPos(const int index)const
{
	switch (index)
	{
	case 0:
		return m_triangle.at(0)->GetPos(0); break;

	case 1:
		return m_triangle.at(0)->GetPos(1); break;

	case 2:
		return m_triangle.at(0)->GetPos(2); break;

	case 3:
		return m_triangle.at(1)->GetPos(2); break;

	default:
		break;
	}
	return math::GetZeroVector();
}

Segment* Square::GetEdge(const int index)const
{
	switch (index)
	{
	case 0:
		return m_triangle.at(0)->GetEdge(0); break;

	case 1:
		return m_triangle.at(0)->GetEdge(1); break;

	case 2:
		return m_triangle.at(1)->GetEdge(1); break;

	case 3:
		return m_triangle.at(1)->GetEdge(2); break;

	default:
		break;
	}
	return nullptr;
}
