#include "graphicer.hpp"

Graphicer::Graphicer(const std::string& file_path) : 
	m_graphic_handle		(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kGraphic, file_path)),
	m_center_pos			(v2d::GetZeroV<Vector2D<int>>()),
	m_scale					(1.0f, 1.0f),
	m_angle					(0.0f),
	m_origin_width			(0),
	m_origin_height			(0),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{
	GetGraphSize(m_graphic_handle, &m_origin_width, &m_origin_height);
}

Graphicer::Graphicer(const int graphic_handle) : 
	m_graphic_handle		(graphic_handle),
	m_center_pos			(v2d::GetZeroV<Vector2D<int>>()),
	m_scale					(1.0f, 1.0f),
	m_angle					(0.0f),
	m_origin_width			(0),
	m_origin_height			(0),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{
	GetGraphSize(m_graphic_handle, &m_origin_width, &m_origin_height);
}

Graphicer::~Graphicer()
{
	DeleteGraph(m_graphic_handle);
}

void Graphicer::Draw() const
{
	DrawRotaGraph3(
		m_center_pos.x,
		m_center_pos.y,
		static_cast<int>(m_origin_width  * 0.5f),
		static_cast<int>(m_origin_height * 0.5f),
		static_cast<double>(m_scale.x), 
		static_cast<double>(m_scale.y), 
		m_angle, 
		m_graphic_handle, TRUE, 
		m_is_invert_horizontal, 
		m_is_invert_vertical);
}