#include "../Calculator/math.hpp"
#include "collision_manager.hpp"

CollisionManager* CollisionManager::m_instance = nullptr;


#pragma region äÓî’
void CollisionManager::Generate()
{
	if (!m_instance)
	{
		m_instance = new CollisionManager;
	}
}

void CollisionManager::Delete()
{
	delete m_instance;
	m_instance = nullptr;
}

CollisionManager::CollisionManager():
	m_collision_calc(nullptr),
	m_fall_range	(nullptr)
{
	m_collision_calc = new CollisionCalculator;
	m_fall_range	 = new FallRange;
}

CollisionManager::~CollisionManager()
{
	delete m_collision_calc;
	delete m_fall_range;
}

void CollisionManager::Update()
{
	std::vector<OnCollideInfoData> on_collide_info = CheckHitAll();
	std::vector<CollideObject*>	   fall_objects	   = CheckFallAll();

	for (auto& itr : on_collide_info)
	{
		itr.owner_object->OnCollide(itr.target_object);
	}

	for (auto& itr : fall_objects)
	{
		itr->OnFallOffStage();
	}
}
#pragma endregion


#pragma region í«â¡ÅEçÌèú
void CollisionManager::AddCollideObject		(CollideObject*		collide_object)
{
	if (std::find(m_collide_objects.begin(), m_collide_objects.end(), collide_object) == m_collide_objects.end())
	{
		m_collide_objects.emplace_back(collide_object);
	}
}

void CollisionManager::DeleteCollideObject	(CollideObject*		collide_object)
{
	if (std::find(m_collide_objects.begin(), m_collide_objects.end(), collide_object) != m_collide_objects.end())
	{
		m_collide_objects.remove(collide_object);
	}
}

void CollisionManager::AddFallObject		(CollideObject*		fall_object)
{
	if (std::find(m_fall_objects.begin(), m_fall_objects.end(), fall_object) == m_fall_objects.end())
	{
		m_fall_objects.emplace_back(fall_object);
	}
}

void CollisionManager::DeleteFallObject		(CollideObject*		fall_object)
{
	if (std::find(m_fall_objects.begin(), m_fall_objects.end(), fall_object) != m_fall_objects.end())
	{
		m_fall_objects.remove(fall_object);
	}
}
#pragma endregion


#pragma region è’ìÀîªíË
std::vector<OnCollideInfoData> CollisionManager::CheckHitAll()
{
	std::vector<OnCollideInfoData> on_collide_info;

	for (auto& owner_obj : m_collide_objects)
	{
		for (auto& target_obj : m_collide_objects)
		{
			// é©êgÇ∆ÇÃìñÇΩÇËîªíËÇÕîÇØÇÈ
			if (owner_obj == target_obj) { continue; }

			if (IsHit(owner_obj, target_obj))
			{
				on_collide_info.emplace_back(OnCollideInfoData(owner_obj, target_obj));
				on_collide_info.emplace_back(OnCollideInfoData(owner_obj, target_obj));
			}
		}
	}

	return on_collide_info;
}

bool CollisionManager::IsHit(CollideObject* owner_object, CollideObject* target_object)
{
	switch (owner_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kLine:			return IsHitLineAndTarget			(dynamic_cast<Line*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kSegment:		return IsHitSegmentAndTarget		(dynamic_cast<Segment*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kPlane:			return IsHitPlaneAndTarget			(dynamic_cast<Plane*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kTriangle:		return IsHitTriangleAndTarget		(dynamic_cast<Triangle*>(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kSquare:		return IsHitSquareAndTarget			(dynamic_cast<Square*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kCircle:		return IsHitCircleAndTarget			(dynamic_cast<Circle*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kCircumference:	return IsHitCircumferenceAndTarget	(dynamic_cast<Circle*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kAABB:			return IsHitAABBAndTarget			(dynamic_cast<AABB*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kOBB:			return IsHitOBBAndTarget			(dynamic_cast<OBB*>		(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kSphere:		return IsHitSphereAndTarget			(dynamic_cast<Sphere*>	(owner_object->GetCollideShape()), target_object); break;
	case ShapeKind::kCapsule:		return IsHitCapsuleAndTarget		(dynamic_cast<Capsule*>	(owner_object->GetCollideShape()), target_object); break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitLineAndTarget			(Line*		line,		CollideObject* target_object)
{
	if (line == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kLine:	return m_collision_calc->IsHitLineAndLine (line, dynamic_cast<Line*> (target_object->GetCollideShape()));	break;
	case ShapeKind::kPlane:	return m_collision_calc->IsHitLineAndPlane(line, dynamic_cast<Plane*>(target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitSegmentAndTarget		(Segment*	segment,	CollideObject* target_object)
{
	if (segment == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:		return m_collision_calc->IsHitSegmentAndSegment			(segment, dynamic_cast<Segment*> (target_object->GetCollideShape()));	break;
	case ShapeKind::kPlane:			return m_collision_calc->IsHitSegmentAndPlane			(segment, dynamic_cast<Plane*>	 (target_object->GetCollideShape()));	break;
	case ShapeKind::kTriangle:		return m_collision_calc->IsHitSegmentAndTriangle		(segment, dynamic_cast<Triangle*>(target_object->GetCollideShape()));	break;
	case ShapeKind::kSquare:		return m_collision_calc->IsHitSegmentAndSquare			(segment, dynamic_cast<Square*>	 (target_object->GetCollideShape()));	break;
	case ShapeKind::kCircumference:	return m_collision_calc->IsHitSegmentAndCircumference	(segment, dynamic_cast<Circle*>	 (target_object->GetCollideShape()));	break;
	case ShapeKind::kCapsule:		return m_collision_calc->IsHitSegmentAndCapsule			(segment, dynamic_cast<Capsule*> (target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitPlaneAndTarget			(Plane*		plane,		CollideObject* target_object)
{
	if (plane == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kLine:		return m_collision_calc->IsHitLineAndPlane		(dynamic_cast<Line*>(target_object->GetCollideShape()),		plane);	break;
	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndPlane	(dynamic_cast<Segment*>(target_object->GetCollideShape()),	plane);	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitPlaneAndCapsule	(plane, dynamic_cast<Capsule*>(target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitTriangleAndTarget		(Triangle*	triangle,	CollideObject* target_object)
{
	if (triangle == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndTriangle(dynamic_cast<Segment*>(target_object->GetCollideShape()), triangle);	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitTriangleAndCapsule(triangle, dynamic_cast<Capsule*>(target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitSquareAndTarget			(Square*	square,		CollideObject* target_object)
{
	if (square == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndSquare(dynamic_cast<Segment*>(target_object->GetCollideShape()), square);	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitSquareAndCapsule(square, dynamic_cast<Capsule*>(target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitCircleAndTarget			(Circle*	circle,		CollideObject* target_object)
{
	return false;
}

bool CollisionManager::IsHitCircumferenceAndTarget	(Circle*	circle,		CollideObject* target_object)
{
	if (circle == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:	return m_collision_calc->IsHitSegmentAndCircumference(dynamic_cast<Segment*>(target_object->GetCollideShape()), circle);	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitCircumferenceAndCapsule(circle, dynamic_cast<Capsule*>(target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitAABBAndTarget			(AABB*		aabb,		CollideObject* target_object)
{
	return false;
}

bool CollisionManager::IsHitOBBAndTarget			(OBB*		obb,		CollideObject* target_object)
{
	return false;
}

bool CollisionManager::IsHitSphereAndTarget			(Sphere*	sphere,		CollideObject* target_object)
{
	if (sphere == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSphere:	return m_collision_calc->IsHitSphereAndSphere (sphere, dynamic_cast<Sphere*> (target_object->GetCollideShape()));	break;
	case ShapeKind::kCapsule:	return m_collision_calc->IsHitSphereAndCapsule(sphere, dynamic_cast<Capsule*>(target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}

bool CollisionManager::IsHitCapsuleAndTarget		(Capsule*	capsule,	CollideObject* target_object)
{
	if (capsule == nullptr) { return false; }

	switch (target_object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kSegment:		return m_collision_calc->IsHitSegmentAndCapsule			(dynamic_cast<Segment*>	(target_object->GetCollideShape()),	capsule);	break;
	case ShapeKind::kPlane:			return m_collision_calc->IsHitPlaneAndCapsule			(dynamic_cast<Plane*>	(target_object->GetCollideShape()),	capsule);	break;
	case ShapeKind::kTriangle:		return m_collision_calc->IsHitTriangleAndCapsule		(dynamic_cast<Triangle*>(target_object->GetCollideShape()),	capsule);	break;
	case ShapeKind::kSquare:		return m_collision_calc->IsHitSquareAndCapsule			(dynamic_cast<Square*>	(target_object->GetCollideShape()),	capsule);	break;
	case ShapeKind::kCircumference: return m_collision_calc->IsHitCircumferenceAndCapsule	(dynamic_cast<Circle*>	(target_object->GetCollideShape()), capsule);	break;
	case ShapeKind::kSphere:		return m_collision_calc->IsHitSphereAndCapsule			(dynamic_cast<Sphere*>	(target_object->GetCollideShape()),	capsule);	break;
	case ShapeKind::kCapsule:		return m_collision_calc->IsHitCapsuleAndCapsule			(capsule, dynamic_cast<Capsule*>(target_object->GetCollideShape()));	break;

	default:
		break;
	}
	return false;
}
#pragma endregion


#pragma region óéâ∫îªíË
std::vector<CollideObject*> CollisionManager::CheckFallAll()
{
	std::vector<CollideObject*> fall_objects;

	for (auto& fall_object : m_fall_objects)
	{
		if (fall_object->GetObjectKind() == ObjectKind::kPlayer)
		{
			if (IsFall(fall_object, m_fall_range->GetPlayerFallRange()))
			{
				fall_objects.emplace_back(fall_object);
			}
		}
		else
		{
			if (IsFall(fall_object, m_fall_range->GetObjectFallRange()))
			{
				fall_objects.emplace_back(fall_object);
			}
		}
	}

	return fall_objects;
}

bool CollisionManager::IsFall(CollideObject* object, CollideShapeBase* fall_range)
{
	switch (object->GetCollideShape()->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		return !math::IsPointAheadOfPlane(dynamic_cast<Capsule*>(object->GetCollideShape())->GetSegment()->GetBeginPos(), 
			dynamic_cast<Plane*>(fall_range));
		break;

	// ñ¢é¿ëï
	default:
		break;
	}
	return false;
}
#pragma endregion
