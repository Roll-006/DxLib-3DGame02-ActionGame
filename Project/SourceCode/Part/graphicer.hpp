#pragma once
#include "../Manager/handle_keeper.hpp"
#include "../Calculation/vector_2d.hpp"
#include "../Window/window.hpp"

class Graphicer final
{
public:
	Graphicer(const std::string& file_path);
	Graphicer(const int graphic_handle);
	~Graphicer();

	void Draw() const;

	void SetCenterPos	(const Vector2D<int>&	pos)	{ m_center_pos	= pos; }
	void SetScale		(const Vector2D<float>& scale)	{ m_scale		= scale; }
	void SetAngle		(const float			angle)	{ m_angle		= angle; }
	void SetWidth		(const int width)				{ m_width		= width; }
	void SetHeigth		(const int height)				{ m_height		= height; }

	[[nodiscard]] int				GetGraphicHandle()	const { return m_graphic_handle; }
	[[nodiscard]] Vector2D<int>		GetCenterPos()		const { return m_center_pos; }
	[[nodiscard]] Vector2D<float>	GetScale()			const { return m_scale; }
	[[nodiscard]] float				GetAngle()			const { return m_angle; }
	[[nodiscard]] int				GetWidth()			const { return m_width; }
	[[nodiscard]] int				GetHeight()			const { return m_height; }

private:
	int				m_graphic_handle;
	Vector2D<int>	m_center_pos;
	Vector2D<float> m_scale;
	float			m_angle;
	int				m_width;
	int				m_height;
};
