#pragma once
#include "i_state.hpp"

template<obj_concepts::ObjT ObjT>
class IMoveState abstract : public IState<ObjT>
{
public:
	virtual ~IMoveState() = default;
};
