#include "graphicer.hpp"

Graphicer::Graphicer(const std::string& file_path) : 
	m_graphic_handle(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kGraphic, file_path)),
	m_center_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_scale			(v2d::GetZeroV<Vector2D<float>>()),
	m_angle			(0.0f),
	m_width			(0),
	m_height		(0)
{
	
}

Graphicer::Graphicer(const int graphic_handle) : 
	m_graphic_handle(graphic_handle),
	m_center_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_scale			(v2d::GetZeroV<Vector2D<float>>()),
	m_angle			(0.0f),
	m_width			(0),
	m_height		(0)
{
	
}

Graphicer::~Graphicer()
{
	DeleteGraph(m_graphic_handle);
}

void Graphicer::Draw() const
{
	// TODO : å„Ç…çÏê¨
}