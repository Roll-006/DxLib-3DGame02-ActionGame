#pragma once
#include "../Part/collider.hpp"

struct ColliderPairData
{
	std::shared_ptr<Collider> owner_collider;
	std::shared_ptr<Collider> target_collider;
	std::optional<VECTOR>	  intersection;		// Õ“Ë‚ÌŒğ“_

	ColliderPairData() :
		owner_collider(nullptr), target_collider(nullptr), intersection(std::nullopt){}

	ColliderPairData(const std::shared_ptr<Collider> owner_collider, const std::shared_ptr<Collider> target_collider, const std::optional<VECTOR> intersection) :
		owner_collider(owner_collider), target_collider(target_collider), intersection(intersection){}
};
