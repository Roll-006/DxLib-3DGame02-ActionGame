#include "../../../Calculator/math.hpp"
#include "obb.hpp"

OBB::OBB(const VECTOR& pos, const VECTOR& dir, const VECTOR& length):
	ShapeBase(ShapeKind::kOBB),
	m_pos		(pos),
	m_dir		(dir),
	m_length		(length)
{
	for (auto& square : m_box.plane)
	{
		square = new Square;
	}

	Init(m_pos, m_dir, m_length);
}

OBB::OBB() :
	ShapeBase(ShapeKind::kOBB),
	m_pos		(math::GetZeroVector()),
	m_dir		(math::GetZeroVector()),
	m_length		(math::GetZeroVector())
{
	for (auto& square : m_box.plane)
	{
		square = new Square;
	}
}

OBB::~OBB()
{
	for (auto& square : m_box.plane)
	{
		delete square;
	}
}

void OBB::Init(const VECTOR& pos, const VECTOR& dir, const VECTOR& length)
{
	m_pos	= pos;
	m_dir = math::GetNormalizedVector(dir);
	m_length	= length;
}

void OBB::Update(const VECTOR& pos, const VECTOR& dir)
{
	m_pos	= pos;
	m_dir = math::GetNormalizedVector(dir);

	CalcVertexPos();
	CalcPlanePos();
}

void OBB::Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num)const
{
	for (auto& square : m_box.plane)
	{
		square->Draw(is_draw_normal_vector, false, alpha_blend_num);
	}

	if (is_draw_frame)
	{
		for (int i = 0; i < BoxData::kEdgeNum; ++i)
		{
			GetEdge(static_cast<BoxData::EdgeKind>(i))->Draw();
		}
	}
}

void OBB::LoadTexture(std::string image_name, BoxData::PlaneKind paste_plane, TextureData::PasteDir paste_dir)
{
	m_box.plane.at(static_cast<int>(paste_plane))->LoadTexture(image_name, paste_dir);
}

Segment* OBB::GetEdge(BoxData::EdgeKind edge_kind)const
{
	switch (edge_kind)
	{
	case BoxData::EdgeKind::kBottomFront:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kBottom))->GetEdge(1); break;

	case BoxData::EdgeKind::kBottomBack:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kBottom))->GetEdge(3); break;

	case BoxData::EdgeKind::kBottomRight:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kBottom))->GetEdge(2); break;

	case BoxData::EdgeKind::kBottomLeft:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kBottom))->GetEdge(0); break;

	case BoxData::EdgeKind::kTopFront:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kTop))	->GetEdge(1); break;

	case BoxData::EdgeKind::kTopBack:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kTop))	->GetEdge(3); break;

	case BoxData::EdgeKind::kTopRight:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kTop))	->GetEdge(0); break;

	case BoxData::EdgeKind::kTopLeft:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kTop))	->GetEdge(2); break;

	case BoxData::EdgeKind::kCenterRightBack:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kBack))	->GetEdge(0); break;

	case BoxData::EdgeKind::kCenterRightFront:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kRight))	->GetEdge(0); break;

	case BoxData::EdgeKind::kCenterLeftFront:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kFront))	->GetEdge(0); break;

	case BoxData::EdgeKind::kCenterLeftBack:
		return m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kLeft))	->GetEdge(0); break;

	default:
		break;
	}
	return nullptr;
}

void OBB::CalcVertexPos()
{
	// 自身を基準とした各軸の向きを取得
	VECTOR z_dir =  m_dir;
	VECTOR y_dir =  math::GetNormalVector(z_dir);
	VECTOR x_dir = -math::GetNormalVector(z_dir, y_dir);

	// 各頂点の座標を取得
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack))  = m_pos - z_dir * m_length.z * 0.5f;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack))  += x_dir             * m_length.x * 0.5f;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack))  -= y_dir             * m_length.y * 0.5f;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightFront)) = m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack))  + z_dir * m_length.z;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftFront))  = m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightFront)) - x_dir * m_length.x;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftBack))   = m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftFront))  - z_dir * m_length.z;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightBack))     = m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack))  + y_dir * m_length.y;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightFront))    = m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightFront)) + y_dir * m_length.y;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftFront))     = m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftFront))  + y_dir * m_length.y;
	m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftBack))      = m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftBack))   + y_dir * m_length.y;
}

void OBB::CalcPlanePos()
{
	m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kFront))->Init(
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightFront)));

	m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kBack))->Init(
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftBack)));

	m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kRight))->Init(
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack)));

	m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kLeft))->Init(
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftFront)));

	m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kBottom))->Init(
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomLeftFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kBottomRightBack)));

	m_box.plane.at(static_cast<int>(BoxData::PlaneKind::kTop))->Init(
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightBack)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopRightFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftFront)),
		m_box.vertex.at(static_cast<int>(BoxData::VertexKind::kTopLeftBack)));
}
