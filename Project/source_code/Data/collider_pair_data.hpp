#pragma once
#include "../Part/collider.hpp"

struct ColliderPairData
{
	std::shared_ptr<Collider> owner_collider;
	std::shared_ptr<Collider> target_collider;

	ColliderPairData() :
		owner_collider(nullptr), target_collider(nullptr){}

	ColliderPairData(const std::shared_ptr<Collider> owner_collider, const std::shared_ptr<Collider> target_collider) :
		owner_collider(owner_collider), target_collider(target_collider){}
};
