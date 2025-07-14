#pragma once
#include "i_state.hpp"

template<obj_concepts::ObjT ObjT>
class IBasisMoveState abstract : public IState<ObjT>
{
public:
	virtual ~IBasisMoveState() = default;
};
