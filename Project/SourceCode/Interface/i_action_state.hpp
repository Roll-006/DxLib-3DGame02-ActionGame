#pragma once
#include "i_state.hpp"

template<obj_concepts::ObjT ObjT>
class IActionState abstract : public IState<ObjT>
{
public:
	virtual ~IActionState() = default;
};
