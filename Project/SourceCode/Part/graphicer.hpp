#pragma once
#include "../Handle/handle_keeper.hpp"
#include "../Calculation/vector_2d.hpp"
#include "../Window/window.hpp"

class Graphicer final
{
public:
	Graphicer(const std::string& file_path);
	Graphicer(const int graphic_handle);
	~Graphicer();

	void Draw(const bool is_draw_graphic_frame = false) const;

	void SetCenterPos		(const Vector2D<int>& pos)			{ m_center_pos				= pos; }
	void SetScale			(const Vector2D<float>& scale);
	void SetScale			(const float scale);
	void SetAngle			(const float angle)					{ m_angle					= angle; }
	void SetBlendMode		(const int blend_mode)				{ m_blend_mode				= blend_mode; }
	void SetBlendNum		(const int blend_num)				{ m_blend_num				= blend_num; }
	void SetInvertHorizontal(const bool is_invert_horizontal)	{ m_is_invert_horizontal	= is_invert_horizontal; }
	void SetInvertVertical	(const bool is_invert_vertical)		{ m_is_invert_vertical		= is_invert_vertical; }

	[[nodiscard]] int				GetGraphicHandle()		const { return m_graphic_handle; }
	[[nodiscard]] Vector2D<int>		GetCenterPos()			const { return m_center_pos; }
	[[nodiscard]] Vector2D<float>	GetScale()				const { return m_scale; }
	[[nodiscard]] float				GetAngle()				const { return m_angle; }
	[[nodiscard]] Vector2D<int>		GetOriginSize()			const { return m_origin_size; }
	[[nodiscard]] Vector2D<int>		GetHalfOriginSize()		const { return m_origin_size * 0.5f; }
	[[nodiscard]] Vector2D<int>		GetSize()				const { return m_size; }
	[[nodiscard]] Vector2D<int>		GetHalfSize()			const { return m_size * 0.5f; }
	[[nodiscard]] int				GetBlendMode()			const { return m_blend_mode; }
	[[nodiscard]] int				GetBlendNum()			const { return m_blend_num; }
	[[nodiscard]] bool				IsInvertHorizontal()	const { return m_is_invert_horizontal; }
	[[nodiscard]] bool				IsInvertVertical()		const { return m_is_invert_vertical; }

private:
	int				m_graphic_handle;
	Vector2D<int>	m_center_pos;
	Vector2D<float> m_scale;
	float			m_angle;
	Vector2D<int>   m_origin_size;
	Vector2D<int>   m_size;
	int				m_blend_mode;
	int				m_blend_num;
	bool			m_is_invert_horizontal;
	bool			m_is_invert_vertical;
};
