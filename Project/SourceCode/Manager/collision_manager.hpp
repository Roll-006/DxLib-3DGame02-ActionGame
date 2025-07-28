#pragma once
#include "../Base/singleton_base.hpp"
#include "../Base/physical_obj_base.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Calculation/collision_calculator.hpp"

class CollisionManager final : public SingletonBase<CollisionManager>
{
public:
	void LateUpdate();

	#pragma region 登録・解除
	/// @brief 衝突判定を行うオブジェクトを追加
	void AddCollideObj			(const std::shared_ptr<PhysicalObjBase> collide_obj);
	/// @brief 衝突判定を行うオブジェクトから除外
	void RemoveCollideObj		(const std::string& obj_name);

	/// @brief 衝突判定を無視するコライダーを追加
	void AddIgnoreCollider	 (const std::string& obj_name, const ColliderKind kind);
	/// @brief 衝突判定を無視するコライダーから除外
	void RemoveIgnoreCollider(const std::string& obj_name, const ColliderKind kind);
	#pragma endregion

	/// @brief 衝突判定を適用させる
	[[nodiscard]] bool IsApplyCollide(const std::shared_ptr<PhysicalObjBase> collide_obj, const ColliderKind kind) const;
	[[nodiscard]] bool IsApplyCollide(const std::string& obj_name, const ColliderKind kind) const;

private:
	CollisionManager();
	~CollisionManager() override;

	/// @brief 衝突判定を起こしたコライダーの組み合わせを生成
	std::vector<ColliderPairOneToManyData> CreateHitColliderPairs();

	#pragma region 衝突判定
	bool IsHit					(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitLineAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitSegmentAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitPlaneAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitTriangleAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitSquareAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitAABBAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitOBBAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitSphereAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitCapsuleAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitModelAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	#pragma endregion

private:
	std::vector<std::shared_ptr<PhysicalObjBase>>				m_collide_objects;			// 衝突判定を行うオブジェクト
	std::unordered_map<std::string, std::vector<ColliderKind>>	m_ignore_collide_collider;	// 衝突判定を無視するコライダー
	int m_handle_create_count;

	friend SingletonBase<CollisionManager>;
};
