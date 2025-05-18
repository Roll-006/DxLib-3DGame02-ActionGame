#pragma once
#include <array>
#include <DxLib.h>
#include "square.hpp"
#include "../../Base/shape_base.hpp"

/// @brief 無限に続く平面
class Plane : public ShapeBase
{
public:
	Plane(const VECTOR& normal_vector);
	Plane();
	~Plane();

	void Init(const VECTOR& normal_vector);
	void Update(const VECTOR& pos);

	/// @brief 描画
	/// @param is_draw_normal_vector 法線ベクトルを描画するかどうか
	/// @param is_draw_frame 枠を描画させるかどうか
	/// @param alpha_blend_num 透過値 (0～255 : 0で完全透過)
	/// @param draw_edge_length 描画時の直線の長さ(実際には無限に続く)
	void Draw(bool is_draw_normal_vector, bool is_draw_frame, int alpha_blend_num, float draw_edge_length)const;

	VECTOR GetPos()const     { return m_pos; }
	VECTOR GetNormalVector()const { return m_normal_vector; }

private:
	VECTOR  m_pos;			// 平面上の基準座標
	VECTOR  m_normal_vector;	// 法線ベクトル
	Square* m_square;
};