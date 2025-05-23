#pragma once
#include <string>
#include <vector>
#include <list>

#include "../Base/singleton_base.hpp"
#include "../Data/collide_obj_pair_data.hpp"
#include "../Calculation/collision_calculator.hpp"

class CollisionManager final : public SingletonBase<CollisionManager>
{
public:
	void Update();

	/// @brief 衝突判定を行うオブジェクトを追加
	void AddCollideObj(std::shared_ptr<CollideObjBase> collide_obj);
	/// @brief 衝突判定を行うオブジェクトを解除
	void RemoveCollideObj(std::shared_ptr<CollideObjBase> collide_obj);

	/// @brief 衝突判定を無視するオブジェクトを追加
	void AddIgnoreObj(std::string obj_name);
	/// @brief 衝突判定を無視するオブジェクトを解除
	void RemoveIgnoreObj(std::string obj_name);

private:
	CollisionManager();
	~CollisionManager()override;

	/// @brief 衝突判定を起こしたオブジェクトの組み合わせを生成
	/// @return 衝突を起こした全てのペア
	std::vector<CollideObjPairData> MakeHitObjPairs();

	#pragma region 衝突判定
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
	std::list<std::shared_ptr<CollideObjBase>> m_collide_objects;	// 衝突判定を行うオブジェクト
	std::list<std::string> m_ignore_object_name;					// 衝突判定を無視するオブジェクト

	friend SingletonBase<CollisionManager>;
};
