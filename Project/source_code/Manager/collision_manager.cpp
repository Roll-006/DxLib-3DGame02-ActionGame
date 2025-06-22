#include "collision_manager.hpp"

CollisionManager::CollisionManager()
{
	// 処理なし
}

CollisionManager::~CollisionManager()
{
	// 処理なし
}

void CollisionManager::Update()
{
	const auto collider_pairs = MakeHitColliderPairs();

	// 衝突したコライダーのオーナーオブジェクトの処理を実行
	for (const auto& collider_pair : collider_pairs)
	{
		collider_pair.owner_collider->GetOwnerObj()->OnCollide(collider_pair);
	}
}


#pragma region 登録・解除
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

void CollisionManager::AddIgnoreCollider(const std::string& obj_name, const ColliderKind kind)
{
	if (std::find(m_ignore_collide_collider[obj_name].begin(), m_ignore_collide_collider[obj_name].end(), kind) == m_ignore_collide_collider[obj_name].end())
	{
		m_ignore_collide_collider[obj_name].emplace_back(kind);
	}
}

void CollisionManager::RemoveIgnoreCollider(const std::string& obj_name, const ColliderKind kind)
{
	if (std::find(m_ignore_collide_collider[obj_name].begin(), m_ignore_collide_collider[obj_name].end(), kind) != m_ignore_collide_collider[obj_name].end())
	{
		erase(m_ignore_collide_collider[obj_name], kind);
	}
}
#pragma endregion


bool CollisionManager::IsApplyCollide(const std::shared_ptr<PhysicalObjBase> collide_obj, const ColliderKind kind) const
{
	return IsApplyCollide(collide_obj->GetName(), kind);
}

bool CollisionManager::IsApplyCollide(const std::string& obj_name, const ColliderKind kind) const
{
	for (const auto& [ignore_name, ignore_collider] : m_ignore_collide_collider)
	{
		const auto itr = std::find(ignore_collider.begin(), ignore_collider.end(), kind);
		if (itr != ignore_collider.end() && obj_name == ignore_name)
		{
			return false;
		}
	}

	return true;
}


std::vector<ColliderPairData> CollisionManager::MakeHitColliderPairs()
{
	std::vector<ColliderPairData> collider_pair;

	// オブジェクトが持つすべてのコライダーの衝突判定を行う
	// TODO : 後に軽量化
	for (const auto& owner_obj : m_collide_objects)
	{
		for (const auto& owner_obj_collider : owner_obj->GetColliderAll())
		{
			// 衝突が許可されている場合のみ処理を続行
			if (!IsApplyCollide(owner_obj->GetName(), owner_obj_collider->GetColliderKind())) { continue; }

			for (const auto& target_obj : m_collide_objects)
			{
				// 自身との当たり判定は避ける
				if (owner_obj == target_obj) { continue; }

				for (const auto& target_obj_collider : target_obj->GetColliderAll())
				{
					// 衝突が許可されている場合のみ処理を続行
					if (!IsApplyCollide(target_obj->GetName(), target_obj_collider->GetColliderKind())) { continue; }

					if (IsHit(*owner_obj_collider, *target_obj_collider))
					{
						collider_pair.emplace_back(ColliderPairData(owner_obj_collider, target_obj_collider));
						collider_pair.emplace_back(ColliderPairData(owner_obj_collider, target_obj_collider));
					}
				}
			}
		}
	}

	return collider_pair;
}


#pragma region 衝突判定
bool CollisionManager::IsHit(const Collider& owner_collider, const Collider& target_collider)
{
	const auto shape = owner_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return IsHitModelAndTarget(owner_collider.GetModelHandle(), target_collider);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kLine:			return IsHitLineAndTarget		(*dynamic_cast<Line*>	 (shape), target_collider); break;
	case ShapeKind::kSegment:		return IsHitSegmentAndTarget	(*dynamic_cast<Segment*> (shape), target_collider); break;
	case ShapeKind::kPlane:			return IsHitPlaneAndTarget		(*dynamic_cast<Plane*>	 (shape), target_collider); break;
	case ShapeKind::kTriangle:		return IsHitTriangleAndTarget	(*dynamic_cast<Triangle*>(shape), target_collider); break;
	case ShapeKind::kSquare:		return IsHitSquareAndTarget		(*dynamic_cast<Square*>	 (shape), target_collider); break;
	case ShapeKind::kAABB:			return IsHitAABBAndTarget		(*dynamic_cast<AABB*>	 (shape), target_collider); break;
	case ShapeKind::kOBB:			return IsHitOBBAndTarget		(*dynamic_cast<OBB*>	 (shape), target_collider); break;
	case ShapeKind::kSphere:		return IsHitSphereAndTarget		(*dynamic_cast<Sphere*>	 (shape), target_collider); break;
	case ShapeKind::kCapsule:		return IsHitCapsuleAndTarget	(*dynamic_cast<Capsule*> (shape), target_collider); break;

	default:break;
	}
	return false;
}

bool CollisionManager::IsHitLineAndTarget(const Line& line, const Collider& target_collider)
{
	const auto shape = target_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return false;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kLine:	return collision::IsHitLineAndLine (line, *dynamic_cast<Line*> (shape));	break;
	case ShapeKind::kPlane:	return collision::IsHitLineAndPlane(line, *dynamic_cast<Plane*>(shape));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitSegmentAndTarget(const Segment& segment, const Collider& target_collider)
{
	const auto shape = target_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return MV1CollCheck_LineDim(target_collider.GetModelHandle(), -1, segment.GetBeginPos(), segment.GetEndPos()).HitNum;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kSegment:	return collision::IsHitSegmentAndSegment (segment, *dynamic_cast<Segment*>	(shape));	break;
	case ShapeKind::kPlane:		return collision::IsHitSegmentAndPlane	 (segment, *dynamic_cast<Plane*>	(shape));	break;
	case ShapeKind::kTriangle:	return collision::IsHitSegmentAndTriangle(segment, *dynamic_cast<Triangle*>	(shape));	break;
	case ShapeKind::kSquare:	return collision::IsHitSegmentAndSquare	 (segment, *dynamic_cast<Square*>	(shape));	break;
	case ShapeKind::kCapsule:	return collision::IsHitSegmentAndCapsule (segment, *dynamic_cast<Capsule*>	(shape));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitPlaneAndTarget(const Plane& plane, const Collider& target_collider)
{
	const auto shape = target_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return false;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitPlaneAndCapsule(plane, *dynamic_cast<Capsule*> (shape));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitTriangleAndTarget(const Triangle& triangle, const Collider& target_collider)
{
	const auto shape = target_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		// TODO : 頂点の始点順は確認していないので要検証
		return MV1CollCheck_Triangle(target_collider.GetModelHandle(), -1, triangle.GetPos(0), triangle.GetPos(1), triangle.GetPos(2)).HitNum;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitTriangleAndCapsule(triangle, *dynamic_cast<Capsule*> (shape));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitSquareAndTarget(const Square& square, const Collider& target_collider)
{
	const auto shape = target_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return false;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitSquareAndCapsule(square, *dynamic_cast<Capsule*> (shape));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitAABBAndTarget(const AABB& aabb, const Collider& target_collider)
{
	return false;
}

bool CollisionManager::IsHitOBBAndTarget(const OBB& obb, const Collider& target_collider)
{
	return false;
}

bool CollisionManager::IsHitSphereAndTarget(const Sphere& sphere, const Collider& target_collider)
{
	const auto shape = target_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return MV1CollCheck_Sphere(target_collider.GetModelHandle(), -1, sphere.GetPos(), sphere.GetRadius()).HitNum;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kSphere:	return collision::IsHitSphereAndSphere (sphere, *dynamic_cast<Sphere*> (shape));	break;
	case ShapeKind::kCapsule:	return collision::IsHitSphereAndCapsule(sphere, *dynamic_cast<Capsule*>(shape));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitCapsuleAndTarget(const Capsule& capsule, const Collider& target_collider)
{
	const auto shape = target_collider.GetShape().get();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		const auto segment = capsule.GetSegment();
		return MV1CollCheck_Capsule(target_collider.GetModelHandle(), -1, segment.GetBeginPos(), segment.GetEndPos(), capsule.GetRadius()).HitNum;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitCapsuleAndCapsule(capsule, *dynamic_cast<Capsule*> (shape));	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitModelAndTarget(const int model_handle, const Collider& target_collider)
{
	return false;
}
#pragma endregion
