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
	void AddCollideObj(std::shared_ptr<PhysicalObjBase> collide_obj);
	/// @brief 衝突判定を行うオブジェクトから除外
	void RemoveCollideObj(std::shared_ptr<PhysicalObjBase> collide_obj);

	/// @brief 衝突判定を無視するオブジェクトを追加
	void AddIgnoreObj(std::string obj_name);
	/// @brief 衝突判定を無視するオブジェクトから除外
	void RemoveIgnoreObj(std::string obj_name);

private:
	CollisionManager();
	~CollisionManager() override;

	/// @brief 衝突判定を起こしたオブジェクトの組み合わせを生成
	/// @return 衝突を起こした全てのペア
	std::vector<CollideObjPairData> MakeHitObjPairs();

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
	std::list<std::shared_ptr<PhysicalObjBase>> m_collide_objects;	// 衝突判定を行うオブジェクト
	std::list<std::string> m_ignore_obj_name;						// 衝突判定を無視するオブジェクト

	friend SingletonBase<CollisionManager>;
};
