#pragma once
#include "../Base/collide_obj_base.hpp"

struct CollideObjPairData
{
	std::shared_ptr<CollideObjBase> owner_obj;
	std::shared_ptr<CollideObjBase> target_obj;

	CollideObjPairData() : 
		owner_obj(nullptr), target_obj(nullptr){}

	CollideObjPairData(const std::shared_ptr<CollideObjBase> owner_obj, const std::shared_ptr<CollideObjBase> target_obj) :
		owner_obj(owner_obj), target_obj(target_obj){}
};
