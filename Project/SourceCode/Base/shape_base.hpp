#pragma once
#include <memory>
#include <DxLib.h>

#include "../Data/Kind/shape_kind.hpp"
#include "../Calculation/math.hpp"

class ShapeBase abstract
{
public:
	ShapeBase(const ShapeKind shape_kind) : m_shape_kind(shape_kind){}

	virtual ~ShapeBase() = default;

	/// @brief 描画
	/// @param is_draw_frame フレームを描画させるかどうか (一次元図形では効果なし)
	/// @param alpha_blend_num 透過値 (0～255 : 0で完全透過) (一次元図形では効果なし)
	/// @param frame_color フレーム色
	virtual void Draw(const bool is_draw_frame, const int alpha_blend_num, const unsigned int frame_color) const abstract;
	
	virtual void Move(const VECTOR& velocity) abstract;

	[[nodiscard]] ShapeKind GetShapeKind() const { return m_shape_kind; }

private:
	ShapeKind m_shape_kind;
};
