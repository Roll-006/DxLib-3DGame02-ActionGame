#pragma once
#include "../Base/physical_obj_base.hpp"

struct CollideObjPairData
{
	std::shared_ptr<PhysicalObjBase> owner_obj;
	std::shared_ptr<PhysicalObjBase> target_obj;

	CollideObjPairData() : 
		owner_obj(nullptr), target_obj(nullptr){}

	CollideObjPairData(const std::shared_ptr<PhysicalObjBase> owner_obj, const std::shared_ptr<PhysicalObjBase> target_obj) :
		owner_obj(owner_obj), target_obj(target_obj){}
};
