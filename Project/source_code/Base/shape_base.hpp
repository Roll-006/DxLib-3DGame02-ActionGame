#pragma once
#include <memory>

#include "../Data/Kind/shape_kind.hpp"
#include "../Support/transform.hpp"

class ShapeBase
{
public:
	ShapeBase(ShapeKind shape_kind) : 
		m_shape_kind(shape_kind)
		//m_transform	(std::make_shared<Transform>(v3d::GetZeroVector()))
	{ }

	virtual ~ShapeBase() = default;

	[[nodiscard]] ShapeKind GetShapeKind()const { return m_shape_kind; }
	//[[nodiscard]] std::shared_ptr<Transform> GetTransform() { return m_transform; }

protected:
	//::shared_ptr<Transform> m_transform;

private:
	ShapeKind m_shape_kind;
};
