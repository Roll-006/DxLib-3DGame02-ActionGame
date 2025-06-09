#pragma once
#include "../../Base/shape_base.hpp"

class Segment final : public ShapeBase
{
public:
	Segment(const VECTOR& begin_pos, const VECTOR& end_pos);
	Segment(const VECTOR& begin_pos, const VECTOR& dir, const float length);
	Segment();
	~Segment() override;

	/// @brief 描画
	/// @param is_draw_frame フレームを描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
	/// @param frame_color フレーム色
	void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const override;

	void Move(const VECTOR& velocity) override;

	void SetPos(const VECTOR& begin_pos);
	void SetLength(const float length);

	/// @brief 直線上の点を取得
	/// @param length 始点からの長さ
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
