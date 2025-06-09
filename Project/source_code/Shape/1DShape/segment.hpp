#pragma once
#include "../../Base/shape_base.hpp"

class Segment final : public ShapeBase
{
public:
	Segment(const VECTOR& begin_pos, const VECTOR& end_pos);
	Segment(const VECTOR& begin_pos, const VECTOR& dir, const float length);
	Segment();
	~Segment() override;

	void Draw(const unsigned int color) const;

	void Move(const VECTOR& velocity) override;

	void SetPos(const VECTOR& begin_pos);
	void SetLength(const float length);

	/// @brief íºê¸è„ÇÃì_ÇéÊìæ
	/// @param length énì_Ç©ÇÁÇÃí∑Ç≥
	[[nodiscard]] VECTOR GetPoint(const float length) const { return m_begin_pos + m_dir * length; }

	[[nodiscard]] VECTOR GetBeginPos()const { return m_begin_pos; }
	[[nodiscard]] VECTOR GetEndPos()  const { return m_end_pos; }
	[[nodiscard]] VECTOR GetDir()	  const { return m_dir; }
	[[nodiscard]] float  GetLength()  const { return m_length; }

private:
	VECTOR m_begin_pos;
	VECTOR m_end_pos;
	VECTOR m_dir;
	float  m_length;
};
