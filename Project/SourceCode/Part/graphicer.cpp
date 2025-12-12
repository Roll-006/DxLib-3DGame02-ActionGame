#include "graphicer.hpp"

Graphicer::Graphicer(const std::string& file_path) : 
	m_graphic_handle		(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kGraphic, file_path)),
	m_center_pos			(v2d::GetZeroV<Vector2D<int>>()),
	m_scale					(1.0f, 1.0f),
	m_angle					(0.0f),
	m_origin_size			(),
	m_size					(),
	m_blend_mode			(DX_BLENDMODE_ALPHA),
	m_blend_num				(UCHAR_MAX),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{
	int width, height;
	GetGraphSize(m_graphic_handle, &width, &height);
	m_origin_size = m_size = { width, height };
}

Graphicer::Graphicer(const int graphic_handle) : 
	m_graphic_handle		(graphic_handle),
	m_center_pos			(v2d::GetZeroV<Vector2D<int>>()),
	m_scale					(1.0f, 1.0f),
	m_angle					(0.0f),
	m_origin_size			(),
	m_size					(),
	m_blend_mode			(DX_BLENDMODE_ALPHA),
	m_blend_num				(UCHAR_MAX),
	m_is_invert_horizontal	(false),
	m_is_invert_vertical	(false)
{
	int width, height;
	GetGraphSize(m_graphic_handle, &width, &height);
	m_origin_size = m_size = { width, height };
}

Graphicer::~Graphicer()
{
	if (m_graphic_handle != -1)
	{
		HandleKeeper::GetInstance()->RemoveHandle(HandleKind::kGraphic, m_graphic_handle);
		DeleteGraph(m_graphic_handle);
		m_graphic_handle = -1;
	}
}

void Graphicer::Draw(const bool is_draw_graphic_frame) const
{
	if (m_blend_num > 0)
	{
		SetDrawBlendMode(m_blend_mode, m_blend_num);

		DrawRotaGraphFast3(
			m_center_pos.x,
			m_center_pos.y,
			static_cast<int>(m_origin_size.x * 0.5f),
			static_cast<int>(m_origin_size.y * 0.5f),
			static_cast<double>(m_scale.x),
			static_cast<double>(m_scale.y),
			m_angle,
			m_graphic_handle,
			TRUE,
			m_is_invert_horizontal,
			m_is_invert_vertical);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (is_draw_graphic_frame)
	{
		DrawBox(
			static_cast<int>(m_center_pos.x - m_size.x * 0.5f),
			static_cast<int>(m_center_pos.y - m_size.y * 0.5f),
			static_cast<int>(m_center_pos.x + m_size.x * 0.5f),
			static_cast<int>(m_center_pos.y + m_size.y * 0.5f),
			0xff0000, FALSE);
	}
}

void Graphicer::SetScale(const Vector2D<float>& scale)
{
	m_scale = scale;
	m_size = m_origin_size * m_scale;
}

void Graphicer::SetScale(const float scale)
{
	m_scale = { scale, scale };
	m_size = m_origin_size * m_scale;
}
