#pragma once
#include "../Part/collider.hpp"

/// @brief 衝突対象のコライダーデータ
struct TargetColliderData
{
	std::shared_ptr<Collider> collider;
	std::optional<VECTOR>	  intersection;		// 衝突時の交点

	TargetColliderData() : 
		collider(nullptr), intersection(std::nullopt) {}

	TargetColliderData(const std::shared_ptr<Collider> collider, const std::optional<VECTOR> intersection) :
		collider(collider), intersection(intersection) {}
};

/// @brief 一対一の衝突コライダーデータ
struct ColliderPairOneToOneData
{
	std::shared_ptr<Collider> owner_collider;
	std::shared_ptr<Collider> target_collider;
	std::optional<VECTOR>	  intersection;		// 衝突時の交点

	ColliderPairOneToOneData() :
		owner_collider(nullptr), target_collider(nullptr), intersection(std::nullopt) {}

	ColliderPairOneToOneData(const std::shared_ptr<Collider> owner_collider, const std::shared_ptr<Collider> target_collider, const std::optional<VECTOR> intersection) :
		owner_collider(owner_collider), target_collider(target_collider), intersection(intersection) {}
};

/// @brief 一対多の衝突コライダーデータ
struct ColliderPairOneToManyData
{
	std::shared_ptr<Collider>		owner_collider;
	std::vector<TargetColliderData>	target_data;

	ColliderPairOneToManyData() : 
		owner_collider(nullptr) {}

	ColliderPairOneToManyData(const std::shared_ptr<Collider> owner_collider, const std::vector<TargetColliderData> target_data) :
		owner_collider(owner_collider), target_data(target_data) {}
};
