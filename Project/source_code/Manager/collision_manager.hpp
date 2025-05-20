#pragma once
#include <string>
#include <vector>
#include <list>

#include "../Base/singleton_base.hpp"
#include "../Data/collide_obj_pair_data.hpp"
#include "../Calculation/collision_calculator.hpp"

class CollisionManager : public SingletonBase<CollisionManager>
{
public:
	void Update();

	/// @brief 衝突判定を行うオブジェクトを追加する
	void AddCollideObject(std::shared_ptr<CollideObjBase> collide_obj);

	/// @brief 衝突判定を行うオブジェクトを削除する
	void RemoveCollideObject(std::shared_ptr<CollideObjBase> collide_obj);

private:
	CollisionManager();
	~CollisionManager();

	#pragma region 衝突判定
	std::vector<CollideObjPairData> CheckHitAll();
	bool IsHit						(const CollideObjBase&	owner_obj,	const CollideObjBase& target_obj);
	bool IsHitLineAndTarget			(const Line&			line,		const CollideObjBase& target_obj);
	bool IsHitSegmentAndTarget		(const Segment&			segment,	const CollideObjBase& target_obj);
	bool IsHitPlaneAndTarget		(const Plane&			plane,		const CollideObjBase& target_obj);
	bool IsHitTriangleAndTarget		(const Triangle&		triangle,	const CollideObjBase& target_obj);
	bool IsHitSquareAndTarget		(const Square&			square,		const CollideObjBase& target_obj);
	bool IsHitAABBAndTarget			(const AABB&			aabb,		const CollideObjBase& target_obj);
	bool IsHitOBBAndTarget			(const OBB&				obb,		const CollideObjBase& target_obj);
	bool IsHitSphereAndTarget		(const Sphere&			sphere,		const CollideObjBase& target_obj);
	bool IsHitCapsuleAndTarget		(const Capsule&			capsule,	const CollideObjBase& target_obj);
	#pragma endregion

private:
	std::list<std::shared_ptr<CollideObjBase>> m_collide_objects;

	friend SingletonBase<CollisionManager>;
};
