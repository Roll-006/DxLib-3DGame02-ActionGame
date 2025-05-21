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

	[[nodiscard]] ShapeKind GetShapeKind()const { return m_shape_kind; }

private:
	ShapeKind m_shape_kind;
};
