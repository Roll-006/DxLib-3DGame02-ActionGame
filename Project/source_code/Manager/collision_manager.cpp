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
	std::vector<CollideObjPairData> collide_obj_pair = MakeHitObjPair();

	for (auto& obj : collide_obj_pair)
	{
		obj.owner_obj->OnCollide(*obj.target_obj);
	}
}


#pragma region ìoò^ÅEâèú
void CollisionManager::AddCollideObj(std::shared_ptr<CollideObjBase> collide_obj)
{
	if (!std::count(m_collide_objects.begin(), m_collide_objects.end(), collide_obj))
	{
		m_collide_objects.emplace_back(collide_obj);
	}
}

void CollisionManager::RemoveCollideObj(std::shared_ptr<CollideObjBase> collide_obj)
{
	if (std::count(m_collide_objects.begin(), m_collide_objects.end(), collide_obj))
	{
		m_collide_objects.remove(collide_obj);
	}
}

void CollisionManager::AddIgnoreObj(std::string obj_name)
{
	if (!std::count(m_ignore_objects.begin(), m_ignore_objects.end(), obj_name))
	{
		m_ignore_objects.emplace_back(obj_name);
	}
}

void CollisionManager::RemoveIgnoreObj(std::string obj_name)
{
	if (std::count(m_ignore_objects.begin(), m_ignore_objects.end(), obj_name))
	{
		m_ignore_objects.remove(obj_name);
	}
}
#pragma endregion


#pragma region è’ìÀîªíË
std::vector<CollideObjPairData> CollisionManager::MakeHitObjPair()
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

bool CollisionManager::IsHit(const CollideObjBase& owner_obj, const CollideObjBase& target_obj)
{
	switch (owner_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kLine:			return IsHitLineAndTarget			(*dynamic_cast<Line*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kSegment:		return IsHitSegmentAndTarget		(*dynamic_cast<Segment*>	(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kPlane:			return IsHitPlaneAndTarget			(*dynamic_cast<Plane*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kTriangle:		return IsHitTriangleAndTarget		(*dynamic_cast<Triangle*>	(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kSquare:		return IsHitSquareAndTarget			(*dynamic_cast<Square*>		(owner_obj.GetCollider().get()), target_obj); break;
	//case ShapeKind::kCircle:		return IsHitCircleAndTarget			(dynamic_cast<Circle*>	(owner_obj->GetCollideShape()), target_obj); break;
	//case ShapeKind::kCircumference:	return IsHitCircumferenceAndTarget	(dynamic_cast<Circle*>	(owner_obj->GetCollideShape()), target_obj); break;
	case ShapeKind::kAABB:			return IsHitAABBAndTarget			(*dynamic_cast<AABB*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kOBB:			return IsHitOBBAndTarget			(*dynamic_cast<OBB*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kSphere:		return IsHitSphereAndTarget			(*dynamic_cast<Sphere*>		(owner_obj.GetCollider().get()), target_obj); break;
	case ShapeKind::kCapsule:		return IsHitCapsuleAndTarget		(*dynamic_cast<Capsule*>	(owner_obj.GetCollider().get()), target_obj); break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitLineAndTarget			(const Line&		line,		const CollideObjBase& target_obj)
{
	switch (target_obj.GetCollider()->GetShapeKind())
	{
	case ShapeKind::kLine:	return collision::IsHitLineAndLine (line, *dynamic_cast<Line*> (target_obj.GetCollider().get()));	break;
	case ShapeKind::kPlane:	return collision::IsHitLineAndPlane(line, *dynamic_cast<Plane*>(target_obj.GetCollider().get()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitSegmentAndTarget		(const Segment&		segment,	const CollideObjBase& target_obj)
{
	switch (target_obj->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:		return m_collision_calc->IsHitSegmentAndSegment			(segment, dynamic_cast<Segment*> (target_obj->GetCollideShape()));	break;
	case ShapeKind::kPlane:			return m_collision_calc->IsHitSegmentAndPlane			(segment, dynamic_cast<Plane*>	 (target_obj->GetCollideShape()));	break;
	case ShapeKind::kTriangle:		return m_collision_calc->IsHitSegmentAndTriangle		(segment, dynamic_cast<Triangle*>(target_obj->GetCollideShape()));	break;
	case ShapeKind::kSquare:		return m_collision_calc->IsHitSegmentAndSquare			(segment, dynamic_cast<Square*>	 (target_obj->GetCollideShape()));	break;
	case ShapeKind::kCircumference:	return m_collision_calc->IsHitSegmentAndCircumference	(segment, dynamic_cast<Circle*>	 (target_obj->GetCollideShape()));	break;
	case ShapeKind::kCapsule:		return m_collision_calc->IsHitSegmentAndCapsule			(segment, dynamic_cast<Capsule*> (target_obj->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitPlaneAndTarget			(const Plane&		plane,		const CollideObjBase& target_obj)
{
	switch (target_obj->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kLine:		return m_collision_calc->IsHitLineAndPlane		(dynamic_cast<Line*>(target_obj->GetCollideShape()),		plane);	break;
	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndPlane	(dynamic_cast<Segment*>(target_obj->GetCollideShape()),	plane);	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitPlaneAndCapsule	(plane, dynamic_cast<Capsule*>(target_obj->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitTriangleAndTarget		(const Triangle&	triangle,	const CollideObjBase& target_obj)
{
	switch (target_obj->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndTriangle(dynamic_cast<Segment*>(target_obj->GetCollideShape()), triangle);	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitTriangleAndCapsule(triangle, dynamic_cast<Capsule*>(target_obj->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitSquareAndTarget			(const Square&		square,		const CollideObjBase& target_obj)
{
	switch (target_obj->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndSquare(dynamic_cast<Segment*>(target_obj->GetCollideShape()), square);	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitSquareAndCapsule(square, dynamic_cast<Capsule*>(target_obj->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

//bool CollisionManager::IsHitCircleAndTarget			(Circle*	circle,		CollideObject* target_obj)
//{
//	return false;
//}
//
//bool CollisionManager::IsHitCircumferenceAndTarget	(Circle*	circle,		CollideObject* target_obj)
//{
//	if (circle == nullptr) { return false; }
//
//	switch (target_obj->GetCollideShape()->GetShapeKind())
//	{
//	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndCircumference(dynamic_cast<Segment*>(target_obj->GetCollideShape()), circle);	break;
//	case ShapeKind::kCapsule:	return m_collision_calc->IsHitCircumferenceAndCapsule(circle, dynamic_cast<Capsule*>(target_obj->GetCollideShape()));	break;
//
//	default:
//		break;
//	}
//	return false;
//}

bool CollisionManager::IsHitAABBAndTarget			(const AABB&		aabb,		const CollideObjBase& target_obj)
{
	return false;
}

bool CollisionManager::IsHitOBBAndTarget			(const OBB&			obb,		const CollideObjBase& target_obj)
{
	return false;
}

bool CollisionManager::IsHitSphereAndTarget			(const Sphere&		sphere,		const CollideObjBase& target_obj)
{
	switch (target_obj->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSphere:	return m_collision_calc->IsHitSphereAndSphere (sphere, dynamic_cast<Sphere*> (target_obj->GetCollideShape()));	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitSphereAndCapsule(sphere, dynamic_cast<Capsule*>(target_obj->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitCapsuleAndTarget		(const Capsule&		capsule,	const CollideObjBase& target_obj)
{
	switch (target_obj->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:		return m_collision_calc->IsHitSegmentAndCapsule			(dynamic_cast<Segment*>	(target_obj->GetCollideShape()),	capsule);	break;
	case ShapeKind::kPlane:			return m_collision_calc->IsHitPlaneAndCapsule			(dynamic_cast<Plane*>	(target_obj->GetCollideShape()),	capsule);	break;
	case ShapeKind::kTriangle:		return m_collision_calc->IsHitTriangleAndCapsule		(dynamic_cast<Triangle*>(target_obj->GetCollideShape()),	capsule);	break;
	case ShapeKind::kSquare:		return m_collision_calc->IsHitSquareAndCapsule			(dynamic_cast<Square*>	(target_obj->GetCollideShape()),	capsule);	break;
	case ShapeKind::kCircumference: return m_collision_calc->IsHitCircumferenceAndCapsule	(dynamic_cast<Circle*>	(target_obj->GetCollideShape()), capsule);	break;
	case ShapeKind::kSphere:		return m_collision_calc->IsHitSphereAndCapsule			(dynamic_cast<Sphere*>	(target_obj->GetCollideShape()),	capsule);	break;
	case ShapeKind::kCapsule:		return m_collision_calc->IsHitCapsuleAndCapsule			(capsule, dynamic_cast<Capsule*>(target_obj->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}
#pragma endregion
