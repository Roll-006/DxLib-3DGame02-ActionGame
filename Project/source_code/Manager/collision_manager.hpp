#pragma once
#include "../Base/singleton_base.hpp"
#include "../Base/physical_obj_base.hpp"

#include "../Data/collider_pair_data.hpp"
#include "../Manager/obj_manager.hpp"
#include "../Calculation/collision_calculator.hpp"

class CollisionManager final : public SingletonBase<CollisionManager>
{
public:
	void Update();


	#pragma region 登録・解除
	/// @brief 衝突判定を行うオブジェクトを追加
	void AddCollideObj			(const std::shared_ptr<PhysicalObjBase> collide_obj);
	/// @brief 衝突判定を行うオブジェクトから除外
	void RemoveCollideObj		(const std::string& obj_name);

	/// @brief 衝突判定を無視するオブジェクトを追加
	/// @brief このオブジェクトが持つコライダーを一括で追加
	void AddIgnoreObjCollide	(const std::string& obj_name);
	/// @brief 衝突判定を無視するオブジェクトから除外
	/// @brief このオブジェクトが持つコライダーを一括で除外
	void RemoveIgnoreObjCollide	(const std::string& obj_name);
	/// @brief 衝突判定を無視するコライダーを追加
	void AddIgnoreObjCollide	(const std::string& obj_name, const ColliderKind kind);
	/// @brief 衝突判定を無視するコライダーから除外
	void RemoveIgnoreObjCollide	(const std::string& obj_name, const ColliderKind kind);
	#pragma endregion

private:
	CollisionManager();
	~CollisionManager() override;

	/// @brief 衝突判定を起こしたコライダーの組み合わせを生成
	std::vector<ColliderPairData> MakeHitColliderPairs();

	#pragma region 衝突判定
	bool IsHit					(const PhysicalObjBase&	owner_obj,	const PhysicalObjBase& target_obj);
	bool IsHitLineAndTarget		(const Line&			line,		const PhysicalObjBase& target_obj);
	bool IsHitSegmentAndTarget	(const Segment&			segment,	const PhysicalObjBase& target_obj);
	bool IsHitPlaneAndTarget	(const Plane&			plane,		const PhysicalObjBase& target_obj);
	bool IsHitTriangleAndTarget	(const Triangle&		triangle,	const PhysicalObjBase& target_obj);
	bool IsHitSquareAndTarget	(const Square&			square,		const PhysicalObjBase& target_obj);
	bool IsHitAABBAndTarget		(const AABB&			aabb,		const PhysicalObjBase& target_obj);
	bool IsHitOBBAndTarget		(const OBB&				obb,		const PhysicalObjBase& target_obj);
	bool IsHitSphereAndTarget	(const Sphere&			sphere,		const PhysicalObjBase& target_obj);
	bool IsHitCapsuleAndTarget	(const Capsule&			capsule,	const PhysicalObjBase& target_obj);
	#pragma endregion

private:
	std::vector<std::shared_ptr<PhysicalObjBase>>				m_collide_objects;			// 衝突判定を行うオブジェクト
	std::unordered_map<std::string, std::vector<ColliderKind>>	m_ignore_collide_collider;	// 衝突判定を無視するコライダー

	friend SingletonBase<CollisionManager>;
};
