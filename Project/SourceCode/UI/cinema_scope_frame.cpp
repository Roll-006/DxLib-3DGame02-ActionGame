#include "cinema_scope_frame.hpp"

CinemaScopeFrame::CinemaScopeFrame() : 
	m_up_frame_up_left_pos		(v2d::GetZeroV<Vector2D<int>>()),
	m_up_frame_down_right_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_down_frame_up_left_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_down_frame_down_right_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_thickness					(0),
	m_increase					(0.0f)
{

}

CinemaScopeFrame::~CinemaScopeFrame()
{

}

void CinemaScopeFrame::LateUpdate()
{
	if (m_increase > 0)
	{
		math::Increase(m_thickness, static_cast<int>(m_increase), kFrameSize.y, false);
	}
	else
	{
		math::Decrease(m_thickness, static_cast<int>(-m_increase), 0);
	}

	CalcPos();
}

void CinemaScopeFrame::Draw() const
{
	if (m_thickness <= 0) { return; }

	DrawBox(
		m_up_frame_up_left_pos.x, 
		m_up_frame_up_left_pos.y, 
		m_up_frame_down_right_pos.x, 
		m_up_frame_down_right_pos.y,
		0x000000, TRUE);

	DrawBox(
		m_down_frame_up_left_pos.x,
		m_down_frame_up_left_pos.y,
		m_down_frame_down_right_pos.x,
		m_down_frame_down_right_pos.y,
		0x000000, TRUE);
}

void CinemaScopeFrame::SetIncreaseThickness(const float speed)
{
	m_increase = speed;
}

void CinemaScopeFrame::CalcPos()
{
	m_up_frame_up_left_pos		= { 0, 0 };
	m_up_frame_down_right_pos	= { Window::kScreenSize.x, m_thickness };
	m_down_frame_up_left_pos	= { 0, Window::kScreenSize.y - m_thickness };
	m_down_frame_down_right_pos = { Window::kScreenSize.x, Window::kScreenSize.y };
}
