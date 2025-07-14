#pragma once
#include "i_state.hpp"

template<obj_concepts::ObjT ObjT>
class IAttackState abstract : public IState<ObjT>
{
public:
	virtual ~IAttackState() = default;
};
