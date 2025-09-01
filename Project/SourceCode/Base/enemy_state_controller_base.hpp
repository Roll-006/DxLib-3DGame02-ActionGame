#pragma once
#include <typeindex>
#include "../Interface/i_state_controller.hpp"

template<obj_concepts::ObjT ObjT>
class EnemyStateControllerBase abstract : public IStateController<ObjT>
{
public:
	virtual ~EnemyStateControllerBase() = default;

private:

};