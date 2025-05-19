//#pragma once
//#include <string>
//#include <vector>
//#include <list>
//#include "../Object/object_base.hpp"
//#include "collide_object.hpp"
//#include "collision_calculator.hpp"
//#include "collision_information_data.hpp"
//#include "CollideShape/shape_header_aggregator.hpp"
//#include "FallRange/fall_range.hpp"
//
//class CollideObject;
//
//class CollisionManager
//{
//public:
//	static void Generate();
//	static void Delete();
//	static CollisionManager* GetInstance() { return m_instance; }
//
//	void Update();
//
//	/// @brief 衝突判定を行うオブジェクトを追加する
//	void AddCollideObject(CollideObject* collide_object);
//
//	/// @brief 衝突判定を行うオブジェクトを削除する
//	void DeleteCollideObject(CollideObject* collide_object);
//
//	/// @brief 落下判定を行うオブジェクトを追加する
//	void AddFallObject(CollideObject* fall_object);
//
//	/// @brief 落下判定を行うオブジェクトを削除する
//	void DeleteFallObject(CollideObject* fall_object);
//
//private:
//	CollisionManager();
//	~CollisionManager();
//
//	#pragma region 衝突判定
//	std::vector<OnCollideInfoData> CheckHitAll();
//	bool IsHit						(CollideObject* owner_object,	CollideObject* target_object);
//	bool IsHitLineAndTarget			(Line*			line,			CollideObject* target_object);
//	bool IsHitSegmentAndTarget		(Segment*		segment,		CollideObject* target_object);
//	bool IsHitPlaneAndTarget		(Plane*			plane,			CollideObject* target_object);
//	bool IsHitTriangleAndTarget		(Triangle*		triangle,		CollideObject* target_object);
//	bool IsHitSquareAndTarget		(Square*		square,			CollideObject* target_object);
//	bool IsHitCircleAndTarget		(Circle*		circle,			CollideObject* target_object);
//	bool IsHitCircumferenceAndTarget(Circle*		circle,			CollideObject* target_object);
//	bool IsHitAABBAndTarget			(AABB*			aabb,			CollideObject* target_object);
//	bool IsHitOBBAndTarget			(OBB*			obb,			CollideObject* target_object);
//	bool IsHitSphereAndTarget		(Sphere*		sphere,			CollideObject* target_object);
//	bool IsHitCapsuleAndTarget		(Capsule*		capsule,		CollideObject* target_object);
//	#pragma endregion
//
//	#pragma region 落下判定
//	std::vector<CollideObject*> CheckFallAll();
//	bool IsFall(CollideObject* object, CollideShapeBase* fall_range);
//	#pragma endregion
//
//private:
//	std::list<CollideObject*>		m_collide_objects;
//	std::list<CollideObject*>		m_fall_objects;
//	CollisionCalculator*			m_collision_calc;
//
//	FallRange*						m_fall_range;
//
//	static CollisionManager*		m_instance;
//};
