#pragma once
#include <array>

#include "square.hpp"

/// @brief 無限に続く平面
class Plane : public ShapeBase
{
public:
	Plane(const VECTOR& pos, const VECTOR& normal_vector);
	Plane();
	~Plane();

	/// @brief 描画
	/// @param is_draw_normal_vector 法線ベクトルを描画するかどうか
	/// @param is_draw_frame 枠を描画させるかどうか
	/// @param draw_edge_length 描画時の直線の長さ(実際には無限に続く)
	/// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
	void Draw(bool is_draw_normal_vector, bool is_draw_frame, float draw_edge_length, int alpha_blend_num, unsigned int frame_color);

	/// @brief 描画用の四角形を作成
	[[nodiscard]] Square MakeDrawSquare(float edge_length);

	void Move(const VECTOR& velocity);

	void SetPos(const VECTOR& pos){ m_pos = pos; }

	VECTOR GetPos()			const	{ return m_pos; }
	VECTOR GetNormalVector()const	{ return m_normal_vector; }

private:
	VECTOR  m_pos;				// 平面上の基準座標
	VECTOR  m_normal_vector;
};