#include "obb.hpp"

OBB::OBB(const VECTOR& pos, const VECTOR& dir, const VECTOR& length):
	ShapeBase	(ShapeKind::kOBB),
	m_pos		(pos),
	m_dir		(v3d::GetNormalizedVector(dir)),
	m_length	(length)
{
	CalcVertexPos();
	CalcSquarePos();
}

OBB::OBB() :
	ShapeBase	(ShapeKind::kOBB),
	m_pos		(v3d::GetZeroVector()),
	m_dir		(v3d::GetZeroVector()),
	m_length	(v3d::GetZeroVector())
{
	// 処理なし
}

OBB::~OBB()
{
	// 処理なし
}

void OBB::Draw(const bool is_draw_normal_vector, const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color)const
{
	for (auto square : m_box.squares)
	{
		square.Draw(is_draw_normal_vector, false, alpha_blend_num, 0xffffff);
	}

	if (is_draw_frame)
	{
		for (int i = 0; i < BoxData::kEdgeNum; ++i)
		{
			GetEdge(static_cast<EdgeKind>(i)).Draw(frame_color);
		}
	}
}

void OBB::LoadTexture(const std::string& file_path, const SquareKind paste_square, const TextureDirKind texture_dir)
{
	m_box.squares.at(static_cast<int>(paste_square)).LoadTexture(file_path, texture_dir);
}

void OBB::Move(const VECTOR& velocity, const bool is_sync_dir)
{
	m_pos += velocity;

	if (is_sync_dir) { m_dir = v3d::GetNormalizedVector(velocity); }

	CalcVertexPos();
	CalcSquarePos();
}

void OBB::SetPos(const VECTOR& pos)
{
	m_pos = pos;

	CalcVertexPos();
	CalcSquarePos();
}

void OBB::SetDir(const VECTOR& dir)
{
	m_dir = dir;

	CalcVertexPos();
	CalcSquarePos();
}

const Segment& OBB::GetEdge(const EdgeKind edge_kind)const
{
	switch (edge_kind)
	{
	case EdgeKind::kBottomFront:
		return m_box.squares.at(static_cast<int>(SquareKind::kBottom)).	GetEdge(1); break;

	case EdgeKind::kBottomBack:
		return m_box.squares.at(static_cast<int>(SquareKind::kBottom)).	GetEdge(3); break;

	case EdgeKind::kBottomRight:
		return m_box.squares.at(static_cast<int>(SquareKind::kBottom)).	GetEdge(2); break;

	case EdgeKind::kBottomLeft:
		return m_box.squares.at(static_cast<int>(SquareKind::kBottom)).	GetEdge(0); break;

	case EdgeKind::kTopFront:
		return m_box.squares.at(static_cast<int>(SquareKind::kTop)).	GetEdge(1); break;

	case EdgeKind::kTopBack:
		return m_box.squares.at(static_cast<int>(SquareKind::kTop)).	GetEdge(3); break;

	case EdgeKind::kTopRight:
		return m_box.squares.at(static_cast<int>(SquareKind::kTop)).	GetEdge(0); break;

	case EdgeKind::kTopLeft:
		return m_box.squares.at(static_cast<int>(SquareKind::kTop)).	GetEdge(2); break;

	case EdgeKind::kCenterRightBack:
		return m_box.squares.at(static_cast<int>(SquareKind::kBack)).	GetEdge(0); break;

	case EdgeKind::kCenterRightFront:
		return m_box.squares.at(static_cast<int>(SquareKind::kRight)).	GetEdge(0); break;

	case EdgeKind::kCenterLeftFront:
		return m_box.squares.at(static_cast<int>(SquareKind::kFront)).	GetEdge(0); break;

	case EdgeKind::kCenterLeftBack:
		return m_box.squares.at(static_cast<int>(SquareKind::kLeft)).	GetEdge(0); break;

	default:
		break;
	}
	return Segment();
}

void OBB::CalcVertexPos()
{
	// 自身を基準とした各軸の向きを取得
	const VECTOR dir_z =  m_dir;
	const VECTOR dir_y =  math::GetNormalVector(dir_z);
	const VECTOR dir_x = -math::GetNormalVector(dir_z, dir_y);

	// 各頂点の座標を取得
	m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack))  = m_pos - dir_z	* m_length.z * 0.5f;
	m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack))  += dir_x         * m_length.x * 0.5f;
	m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack))  -= dir_y         * m_length.y * 0.5f;
	m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightFront)) = m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack))  + dir_z * m_length.z;
	m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftFront))  = m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightFront)) - dir_x * m_length.x;
	m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftBack))   = m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftFront))  - dir_z * m_length.z;
	m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightBack))     = m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack))  + dir_y * m_length.y;
	m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightFront))    = m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightFront)) + dir_y * m_length.y;
	m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftFront))     = m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftFront))  + dir_y * m_length.y;
	m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftBack))      = m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftBack))   + dir_y * m_length.y;
}

void OBB::CalcSquarePos()
{
	// 各面の座標を取得
	m_box.squares.at(static_cast<int>(SquareKind::kFront)) = Square(
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightFront)));

	m_box.squares.at(static_cast<int>(SquareKind::kBack)) = Square(
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftBack)));

	m_box.squares.at(static_cast<int>(SquareKind::kRight)) = Square(
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack)));

	m_box.squares.at(static_cast<int>(SquareKind::kLeft)) = Square(
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftFront)));

	m_box.squares.at(static_cast<int>(SquareKind::kBottom)) = Square(
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomLeftFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kBottomRightBack)));

	m_box.squares.at(static_cast<int>(SquareKind::kTop)) = Square(
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightBack)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopRightFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftFront)),
		m_box.vertexes.at(static_cast<int>(VertexKind::kTopLeftBack)));
}
