#pragma once
#include "../Base/collide_obj_base.hpp"

struct CollideObjPairData
{
	std::shared_ptr<CollideObjBase> owner_obj;
	std::shared_ptr<CollideObjBase> target_obj;
};
