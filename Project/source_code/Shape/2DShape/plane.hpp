#pragma once
#include <array>

#include "square.hpp"

/// @brief –³ŒÀ‚É‘±‚­•½–Ê
class Plane final : public ShapeBase
{
public:
	Plane(const VECTOR& pos, const VECTOR& normal_vector);
	Plane();
	~Plane();

	/// @brief •`‰æ
	/// @param is_draw_normal_vector –@üƒxƒNƒgƒ‹‚ğ•`‰æ‚·‚é‚©‚Ç‚¤‚©
	/// @param is_draw_frame ˜g‚ğ•`‰æ‚³‚¹‚é‚©‚Ç‚¤‚©
	/// @param draw_edge_length •`‰æ‚Ì’¼ü‚Ì’·‚³(ÀÛ‚É‚Í–³ŒÀ‚É‘±‚­)
	/// @param alpha_blend_num “§‰ß’l (0`255 : 0‚ÅŠ®‘S“§‰ß)
	void Draw(const bool is_draw_normal_vector, const bool is_draw_frame, 
		const float draw_edge_length, const int alpha_blend_num, const unsigned int frame_color) const;

	/// @brief •`‰æ—p‚ÌlŠpŒ`‚ğì¬
	[[nodiscard]] Square&& MakeDrawSquare(const float edge_length) const;

	void Move(const VECTOR& velocity);

	void SetPos(const VECTOR& pos){ m_pos = pos; }

	VECTOR GetPos()			const	{ return m_pos; }
	VECTOR GetNormalVector() const	{ return m_normal_vector; }

private:
	VECTOR  m_pos;				// •½–Êã‚ÌŠî€À•W
	VECTOR  m_normal_vector;
};