#pragma once
#include "i_state.hpp"

template<obj_concepts::ObjT ObjT>
class IWeaponActionState abstract : public IState<ObjT>
{
public:
	virtual ~IWeaponActionState() = default;
};
