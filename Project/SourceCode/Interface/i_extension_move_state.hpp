#pragma once
#include "i_state.hpp"

template<obj_concepts::ObjT ObjT>
class IExtensionMoveState abstract : public IState<ObjT>
{
public:
	virtual ~IExtensionMoveState() = default;
};
