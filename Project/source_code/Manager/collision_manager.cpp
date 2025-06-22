#include "collision_manager.hpp"

CollisionManager::CollisionManager()
{
	// èàóùÇ»Çµ
}

CollisionManager::~CollisionManager()
{
	// èàóùÇ»Çµ
}

void CollisionManager::Update()
{
	const auto collide_obj_pair = MakeHitColliderPairs();

	for (const auto& obj : collide_obj_pair)
	{
		obj.owner_obj->OnCollide(*obj.target_obj);
	}
}


#pragma region ìoò^ÅEâèú
void CollisionManager::AddCollideObj(const std::shared_ptr<PhysicalObjBase> collide_obj)
{
	if (std::find(m_collide_objects.begin(), m_collide_objects.end(), collide_obj) == m_collide_objects.end())
	{
		m_collide_objects.emplace_back(collide_obj);
	}
}

void CollisionManager::RemoveCollideObj(const std::string& obj_name)
{
	const auto physical_obj = std::static_pointer_cast<PhysicalObjBase>(ObjManager::GetInstance()->GetObj(obj_name));

	if (std::find(m_collide_objects.begin(), m_collide_objects.end(), physical_obj) != m_collide_objects.end())
	{
		erase(m_collide_objects, physical_obj);
	}
}

void CollisionManager::AddIgnoreObjCollide(const std::string& obj_name)
{
	const auto physical_obj = std::static_pointer_cast<PhysicalObjBase>(ObjManager::GetInstance()->GetObj(obj_name));
	const auto colliders	= physical_obj->GetColliderAll();

	for (const auto& collider : colliders)
	{
		AddIgnoreObjCollide(obj_name, collider->GetColliderKind());
	}
}

void CollisionManager::RemoveIgnoreObjCollide(const std::string& obj_name)
{
	const auto physical_obj = std::static_pointer_cast<PhysicalObjBase>(ObjManager::GetInstance()->GetObj(obj_name));
	const auto colliders = physical_obj->GetColliderAll();

	for (const auto& collider : colliders)
	{
		RemoveIgnoreObjCollide(obj_name, collider->GetColliderKind());
	}
}

void CollisionManager::AddIgnoreObjCollide(const std::string& obj_name, const ColliderKind kind)
{
	if (std::find(m_ignore_collide_collider[obj_name].begin(), m_ignore_collide_collider[obj_name].end(), kind) == m_ignore_collide_collider[obj_name].end())
	{
		m_ignore_collide_collider[obj_name].emplace_back(kind);
	}
}

void CollisionManager::RemoveIgnoreObjCollide(const std::string& obj_name, const ColliderKind kind)
{
	if (std::find(m_ignore_collide_collider[obj_name].begin(), m_ignore_collide_collider[obj_name].end(), kind) == m_ignore_collide_collider[obj_name].end())
	{
		erase(m_ignore_collide_collider[obj_name], kind);
	}
}
#pragma endregion


#pragma region è’ìÀîªíË
std::vector<ColliderPairData> CollisionManager::MakeHitColliderPairs()
{
	std::vector<ColliderPairData> collide_obj_pair;

	//for (auto& owner_obj : m_collide_objects)
	//{
	//	for (auto& target_obj : m_collide_objects)
	//	{
	//		// é©êgÇ∆ÇÃìñÇΩÇËîªíËÇÕîÇØÇÈ
	//		if (owner_obj == target_obj) { continue; }

	//		if (IsHit(*owner_obj, *target_obj))
	//		{
	//			collide_obj_pair.emplace_back(ColliderPairData(owner_obj, target_obj));
	//			collide_obj_pair.emplace_back(ColliderPairData(owner_obj, target_obj));
	//		}
	//	}
	//}

	return collide_obj_pair;
}

//bool CollisionManager::IsHit(const PhysicalObjBase& owner_obj, const PhysicalObjBase& target_obj)
//{
//	switch (owner_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kLine:			return IsHitLineAndTarget		(*dynamic_cast<Line*>		(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kSegment:		return IsHitSegmentAndTarget	(*dynamic_cast<Segment*>	(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kPlane:			return IsHitPlaneAndTarget		(*dynamic_cast<Plane*>		(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kTriangle:		return IsHitTriangleAndTarget	(*dynamic_cast<Triangle*>	(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kSquare:		return IsHitSquareAndTarget		(*dynamic_cast<Square*>		(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kAABB:			return IsHitAABBAndTarget		(*dynamic_cast<AABB*>		(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kOBB:			return IsHitOBBAndTarget		(*dynamic_cast<OBB*>		(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kSphere:		return IsHitSphereAndTarget		(*dynamic_cast<Sphere*>		(owner_obj.GetCollider().get()), target_obj); break;
//	case ShapeKind::kCapsule:		return IsHitCapsuleAndTarget	(*dynamic_cast<Capsule*>	(owner_obj.GetCollider().get()), target_obj); break;
//
//	default: break;
//	}
//	return false;
//}
//
//bool CollisionManager::IsHitLineAndTarget			(const Line&		line,		const PhysicalObjBase& target_obj)
//{
//	switch (target_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kLine:	return collision::IsHitLineAndLine (line, *dynamic_cast<Line*> (target_obj.GetCollider().get()));	break;
//	case ShapeKind::kPlane:	return collision::IsHitLineAndPlane(line, *dynamic_cast<Plane*>(target_obj.GetCollider().get()));	break;
//
//	default: break;
//	}
//	return false;
//}
//
//bool CollisionManager::IsHitSegmentAndTarget		(const Segment&		segment,	const PhysicalObjBase& target_obj)
//{
//	switch (target_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kSegment:		return collision::IsHitSegmentAndSegment	(segment, *dynamic_cast<Segment*>	(target_obj.GetCollider().get()));	break;
//	case ShapeKind::kPlane:			return collision::IsHitSegmentAndPlane		(segment, *dynamic_cast<Plane*>		(target_obj.GetCollider().get()));	break;
//	case ShapeKind::kTriangle:		return collision::IsHitSegmentAndTriangle	(segment, *dynamic_cast<Triangle*>	(target_obj.GetCollider().get()));	break;
//	case ShapeKind::kSquare:		return collision::IsHitSegmentAndSquare		(segment, *dynamic_cast<Square*>	(target_obj.GetCollider().get()));	break;
//	case ShapeKind::kCapsule:		return collision::IsHitSegmentAndCapsule	(segment, *dynamic_cast<Capsule*>	(target_obj.GetCollider().get()));	break;
//
//	default: break;
//	}
//	return false;
//}
//
//bool CollisionManager::IsHitPlaneAndTarget			(const Plane&		plane,		const PhysicalObjBase& target_obj)
//{
//	switch (target_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kLine:		return collision::IsHitLineAndPlane		(*dynamic_cast<Line*>	(target_obj.GetCollider().get()),	plane);	break;
//	case ShapeKind::kSegment:	return collision::IsHitSegmentAndPlane	(*dynamic_cast<Segment*>(target_obj.GetCollider().get()),	plane);	break;
//	case ShapeKind::kCapsule:	return collision::IsHitPlaneAndCapsule	(plane, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;
//
//	default: break;
//	}
//	return false;
//}
//
//bool CollisionManager::IsHitTriangleAndTarget		(const Triangle&	triangle,	const PhysicalObjBase& target_obj)
//{
//	switch (target_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kSegment:	return collision::IsHitSegmentAndTriangle(*dynamic_cast<Segment*>(target_obj.GetCollider().get()), triangle);	break;
//	case ShapeKind::kCapsule:	return collision::IsHitTriangleAndCapsule(triangle, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;
//
//	default: break;
//	}
//	return false;
//}
//
//bool CollisionManager::IsHitSquareAndTarget			(const Square&		square,		const PhysicalObjBase& target_obj)
//{
//	switch (target_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kSegment:	return collision::IsHitSegmentAndSquare(*dynamic_cast<Segment*>(target_obj.GetCollider().get()), square);	break;
//	case ShapeKind::kCapsule:	return collision::IsHitSquareAndCapsule(square, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;
//
//	default: break;
//	}
//	return false;
//}
//
//bool CollisionManager::IsHitAABBAndTarget			(const AABB&		aabb,		const PhysicalObjBase& target_obj)
//{
//	return false;
//}
//
//bool CollisionManager::IsHitOBBAndTarget			(const OBB&			obb,		const PhysicalObjBase& target_obj)
//{
//	return false;
//}
//
//bool CollisionManager::IsHitSphereAndTarget			(const Sphere&		sphere,		const PhysicalObjBase& target_obj)
//{
//	switch (target_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kSphere:	return collision::IsHitSphereAndSphere (sphere, *dynamic_cast<Sphere*> (target_obj.GetCollider().get()));	break;
//	case ShapeKind::kCapsule:	return collision::IsHitSphereAndCapsule(sphere, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;
//
//	default: break;
//	}
//	return false;
//}
//
//bool CollisionManager::IsHitCapsuleAndTarget		(const Capsule&		capsule,	const PhysicalObjBase& target_obj)
//{
//	switch (target_obj.GetCollider()->GetShapeKind())
//	{
//	case ShapeKind::kSegment:		return collision::IsHitSegmentAndCapsule	(*dynamic_cast<Segment*>	(target_obj.GetCollider().get()),	capsule);	break;
//	case ShapeKind::kPlane:			return collision::IsHitPlaneAndCapsule		(*dynamic_cast<Plane*>		(target_obj.GetCollider().get()),	capsule);	break;
//	case ShapeKind::kTriangle:		return collision::IsHitTriangleAndCapsule	(*dynamic_cast<Triangle*>	(target_obj.GetCollider().get()),	capsule);	break;
//	case ShapeKind::kSquare:		return collision::IsHitSquareAndCapsule		(*dynamic_cast<Square*>		(target_obj.GetCollider().get()),	capsule);	break;
//	case ShapeKind::kSphere:		return collision::IsHitSphereAndCapsule		(*dynamic_cast<Sphere*>		(target_obj.GetCollider().get()),	capsule);	break;
//	case ShapeKind::kCapsule:		return collision::IsHitCapsuleAndCapsule	(capsule, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));			break;
//
//	default: break;
//	}
//	return false;
//}
#pragma endregion
