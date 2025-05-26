#pragma once
#include "../../Base/shape_base.hpp"

class Line final : public ShapeBase
{
public:
	Line(const VECTOR& pos, const VECTOR& dir);
	Line();
	~Line();

	/// @brief •`‰æ
	/// @param draw_length •`‰æ‚·‚éÛ‚Ì’·‚³
	void Draw(const int draw_length, const unsigned int color);

	void Move(const VECTOR& velocity);

	void SetPos(const VECTOR& pos);

	/// @brief ’¼üã‚Ì“_‚ğæ“¾
	/// @param length n“_‚©‚ç‚Ì’·‚³
	[[nodiscard]] VECTOR GetPoint(const float length) const{ return m_pos + m_dir * length; }

	[[nodiscard]] VECTOR GetPos() const	{ return m_pos; }
	[[nodiscard]] VECTOR GetDir() const	{ return m_dir; }

private:
	VECTOR m_pos;
	VECTOR m_dir;
};
