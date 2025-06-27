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
	std::vector<ColliderPairOneToManyData> MakeHitColliderPairs();

	#pragma region 衝突判定
	bool IsHit					(const Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitLineAndTarget		(const Line&	 line,			 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitSegmentAndTarget	(const Segment&	 segment,		 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitPlaneAndTarget	(const Plane&	 plane,			 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitTriangleAndTarget	(const Triangle& triangle,		 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitSquareAndTarget	(const Square&	 square,		 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitAABBAndTarget		(const AABB&	 aabb,			 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitOBBAndTarget		(const OBB&		 obb,			 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitSphereAndTarget	(const Sphere&	 sphere,		 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitCapsuleAndTarget	(const Capsule&	 capsule,		 const Collider& target_collider, std::optional<VECTOR>& intersection);
	bool IsHitModelAndTarget	(const int		 model_handle,	 const Collider& target_collider, std::optional<VECTOR>& intersection);
	#pragma endregion

private:
	std::vector<std::shared_ptr<PhysicalObjBase>>				m_collide_objects;			// 衝突判定を行うオブジェクト
	std::unordered_map<std::string, std::vector<ColliderKind>>	m_ignore_collide_collider;	// 衝突判定を無視するコライダー

	friend SingletonBase<CollisionManager>;
};
