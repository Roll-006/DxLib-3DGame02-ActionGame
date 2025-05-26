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
	std::vector<CollideObjPairData> collide_obj_pair = MakeHitObjPairs();

	for (auto& obj : collide_obj_pair)
	{
		obj.owner_obj->OnCollide(*obj.target_obj);
	}
}


#pragma region ìoò^ÅEâèú
void CollisionManager::AddCollideObj(std::shared_ptr<PhysicalObjBase> collide_obj)
{
	if (!std::count(m_collide_objects.begin(), m_collide_objects.end(), collide_obj))
	{
		m_collide_objects.emplace_back(collide_obj);
	}
}

void CollisionManager::RemoveCollideObj(std::shared_ptr<PhysicalObjBase> collide_obj)
{
	if (std::count(m_collide_objects.begin(), m_collide_objects.end(), collide_obj))
	{
		m_collide_objects.remove(collide_obj);
	}
}

void CollisionManager::AddIgnoreObj(std::string obj_name)
{
	if (!std::count(m_ignore_object_name.begin(), m_ignore_object_name.end(), obj_name))
	{
		m_ignore_object_name.emplace_back(obj_name);
	}
}

void CollisionManager::RemoveIgnoreObj(std::string obj_name)
{
	if (std::count(m_ignore_object_name.begin(), m_ignore_object_name.end(), obj_name))
	{
		m_ignore_object_name.remove(obj_name);
	}
}
#pragma endregion


#pragma region è’ìÀîªíË
std::vector<CollideObjPairData> CollisionManager::MakeHitObjPairs()
{
	std::vector<CollideObjPairData> collide_obj_pair;

	for (auto& owner_obj : m_collide_objects)
	{
		for (auto& target_obj : m_collide_objects)
		{
			// é©êgÇ∆ÇÃìñÇΩÇËîªíËÇÕîÇØÇÈ
			if (owner_obj == target_obj) { continue; }

			if (IsHit(*owner_obj, *target_obj))
			{
				collide_obj_pair.emplace_back(CollideObjPairData(owner_obj, target_obj));
				collide_obj_pair.emplace_back(CollideObjPairData(owner_obj, target_obj));
			}
		}
	}

	return collide_obj_pair;
}

bool CollisionManager::IsHit(const PhysicalObjBase& owner_obj, const PhysicalObjBase& target_obj)
{
	switch (owner_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kLine:			return IsHitLineAndTarget		(*dynamic_cast<Line*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kSegment:		return IsHitSegmentAndTarget	(*dynamic_cast<Segment*>	(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kPlane:			return IsHitPlaneAndTarget		(*dynamic_cast<Plane*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kTriangle:		return IsHitTriangleAndTarget	(*dynamic_cast<Triangle*>	(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kSquare:		return IsHitSquareAndTarget		(*dynamic_cast<Square*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kAABB:			return IsHitAABBAndTarget		(*dynamic_cast<AABB*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kOBB:			return IsHitOBBAndTarget		(*dynamic_cast<OBB*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kSphere:		return IsHitSphereAndTarget		(*dynamic_cast<Sphere*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kCapsule:		return IsHitCapsuleAndTarget	(*dynamic_cast<Capsule*>	(owner_obj.GetCollider().get()), target_obj); break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitLineAndTarget			(const Line&		line,		const PhysicalObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kLine:	return collision::IsHitLineAndLine (line, *dynamic_cast<Line*> (target_obj.GetCollider().get()));	break;
	case ShapeKind::kPlane:	return collision::IsHitLineAndPlane(line, *dynamic_cast<Plane*>(target_obj.GetCollider().get()));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitSegmentAndTarget		(const Segment&		segment,	const PhysicalObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kSegment:		return collision::IsHitSegmentAndSegment	(segment, *dynamic_cast<Segment*>	(target_obj.GetCollider().get()));	break;
	case ShapeKind::kPlane:			return collision::IsHitSegmentAndPlane		(segment, *dynamic_cast<Plane*>		(target_obj.GetCollider().get()));	break;
	case ShapeKind::kTriangle:		return collision::IsHitSegmentAndTriangle	(segment, *dynamic_cast<Triangle*>	(target_obj.GetCollider().get()));	break;
	case ShapeKind::kSquare:		return collision::IsHitSegmentAndSquare		(segment, *dynamic_cast<Square*>	(target_obj.GetCollider().get()));	break;
	case ShapeKind::kCapsule:		return collision::IsHitSegmentAndCapsule	(segment, *dynamic_cast<Capsule*>	(target_obj.GetCollider().get()));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitPlaneAndTarget			(const Plane&		plane,		const PhysicalObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kLine:		return collision::IsHitLineAndPlane		(*dynamic_cast<Line*>	(target_obj.GetCollider().get()),	plane);	break;
	case ShapeKind::kSegment:	return collision::IsHitSegmentAndPlane	(*dynamic_cast<Segment*>(target_obj.GetCollider().get()),	plane);	break;
	case ShapeKind::kCapsule:	return collision::IsHitPlaneAndCapsule	(plane, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitTriangleAndTarget		(const Triangle&	triangle,	const PhysicalObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kSegment:	return collision::IsHitSegmentAndTriangle(*dynamic_cast<Segment*>(target_obj.GetCollider().get()), triangle);	break;
	case ShapeKind::kCapsule:	return collision::IsHitTriangleAndCapsule(triangle, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitSquareAndTarget			(const Square&		square,		const PhysicalObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kSegment:	return collision::IsHitSegmentAndSquare(*dynamic_cast<Segment*>(target_obj.GetCollider().get()), square);	break;
	case ShapeKind::kCapsule:	return collision::IsHitSquareAndCapsule(square, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitAABBAndTarget			(const AABB&		aabb,		const PhysicalObjBase& target_obj)
{
	return false;
}

bool CollisionManager::IsHitOBBAndTarget			(const OBB&			obb,		const PhysicalObjBase& target_obj)
{
	return false;
}

bool CollisionManager::IsHitSphereAndTarget			(const Sphere&		sphere,		const PhysicalObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kSphere:	return collision::IsHitSphereAndSphere (sphere, *dynamic_cast<Sphere*> (target_obj.GetCollider().get()));	break;
	case ShapeKind::kCapsule:	return collision::IsHitSphereAndCapsule(sphere, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitCapsuleAndTarget		(const Capsule&		capsule,	const PhysicalObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kSegment:		return collision::IsHitSegmentAndCapsule	(*dynamic_cast<Segment*>	(target_obj.GetCollider().get()),	capsule);	break;
	case ShapeKind::kPlane:			return collision::IsHitPlaneAndCapsule		(*dynamic_cast<Plane*>		(target_obj.GetCollider().get()),	capsule);	break;
	case ShapeKind::kTriangle:		return collision::IsHitTriangleAndCapsule	(*dynamic_cast<Triangle*>	(target_obj.GetCollider().get()),	capsule);	break;
	case ShapeKind::kSquare:		return collision::IsHitSquareAndCapsule		(*dynamic_cast<Square*>		(target_obj.GetCollider().get()),	capsule);	break;
	case ShapeKind::kSphere:		return collision::IsHitSphereAndCapsule		(*dynamic_cast<Sphere*>		(target_obj.GetCollider().get()),	capsule);	break;
	case ShapeKind::kCapsule:		return collision::IsHitCapsuleAndCapsule	(capsule, *dynamic_cast<Capsule*>(target_obj.GetCollider().get()));			break;

	default: break;
	}
	return false;
}
#pragma endregion
