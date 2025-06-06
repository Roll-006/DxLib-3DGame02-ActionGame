#pragma once
#include "../../Base/shape_base.hpp"

class AABB : public ShapeBase
{
public:
	AABB();
	~AABB() override;

	void Move(const VECTOR& velocity) override;

private:
};