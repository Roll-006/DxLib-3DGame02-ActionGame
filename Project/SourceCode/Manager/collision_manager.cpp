#include "collision_manager.hpp"

CollisionManager::CollisionManager() : 
	m_handle_create_count(-1)
{
	// 処理なし
}

CollisionManager::~CollisionManager()
{
	// 処理なし
}

void CollisionManager::LateUpdate()
{
	const auto collider_pairs = CreateHitColliderPairs();

	// 衝突したコライダーのオーナーオブジェクトの処理を実行
	for (const auto& pair : collider_pairs)
	{
		// レイキャストトリガーの場合は最初に衝突したコライダーのみ衝突したものとする
		if (pair.owner_collider->GetColliderKind() == ColliderKind::kRayCast && pair.owner_collider->IsClosestOnlyHit())
		{
			// コライダーとの距離を取得
			std::unordered_map<int, TargetColliderData> target;
			std::unordered_map<int, float> distance;
			for (int i = 0; i < pair.target_data.size(); ++i)
			{
				if (pair.target_data.at(i).intersection)
				{
					target[i]	= TargetColliderData(pair.target_data.at(i).collider, pair.target_data.at(i).intersection);
					distance[i] = VSize(*pair.target_data.at(i).intersection - std::dynamic_pointer_cast<Segment>(pair.owner_collider->GetShape())->GetBeginPos());
				}
			}

			// 最も近いコライダーのみ判定
			distance = algorithm::Sort(distance, SortKind::kAscending);
			for (const auto& dist : distance)
			{
				pair.owner_collider			  ->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(pair.owner_collider, target.at(dist.first).collider, target.at(dist.first).intersection));
				target.at(dist.first).collider->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(target.at(dist.first).collider, pair.owner_collider, target.at(dist.first).intersection));
				break;
			}
		}
		// レイキャストトリガーでない場合はすべてのコライダーと衝突判定を行う
		else
		{
			for (const auto& target : pair.target_data)
			{
				pair.owner_collider->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(pair.owner_collider, target.collider, target.intersection));
				target.collider	   ->GetOwnerObj()->OnCollide(ColliderPairOneToOneData(target.collider, pair.owner_collider, target.intersection));
			}
		}
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
	const auto physical_obj = ObjManager::GetInstance()->GetObj<PhysicalObjBase>(obj_name);

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
	// 無視するリストに登録されていれば適用しない
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


std::vector<ColliderPairOneToManyData> CollisionManager::CreateHitColliderPairs()
{
	std::vector<ColliderPairOneToManyData> collider_pairs;

	// オブジェクトが持つすべてのコライダーの衝突判定を行う
	// 
	// TODO : 軽量化
	//	・着地用トリガーと頭部トリガーの衝突は無視するなど、ありえない組み合わせは無視
	//	・遠いオブジェクト同士は無視(移動速度は考慮)
	//	・二つのオブジェクトの関係において、前フレームと現在のフレームで座標が同じであった場合は無視
	//	・静的オブジェクト同士は無視(地面と家など)
	//
	for (const auto& owner_obj : m_collide_objects)
	{
		// 非アクティブの場合はスキップ
		if (!owner_obj->IsActive()) { continue; }

		for (const auto& owner_obj_collider : owner_obj->GetColliderAll())
		{
			// 衝突が許可されている場合のみ処理を続行
			if (!IsApplyCollide(owner_obj->GetName(), owner_obj_collider->GetColliderKind())) { continue; }

			for (const auto& target_obj : m_collide_objects)
			{
				// 自身との当たり判定は避ける
				if (owner_obj == target_obj) { continue; }

				// 非アクティブの場合はスキップ
				if (!target_obj->IsActive()) { continue; }

				for (const auto& target_obj_collider : target_obj->GetColliderAll())
				{
					// 衝突が許可されている場合のみ処理を続行
					if (!IsApplyCollide(target_obj->GetName(), target_obj_collider->GetColliderKind())) { continue; }

					// 衝突判定
					std::optional<VECTOR> intersection;
					if (IsHit(*owner_obj_collider, *target_obj_collider, intersection))
					{
						// MEMO : 確認用
						if (target_obj->GetName() == ObjName.BULLET)
						{
							int a = 0;
						}

						// 指定のオーナーのデータコンテナがまだない場合は新たに作成
						bool is_maked = std::any_of(collider_pairs.begin(), collider_pairs.end(), [=](const ColliderPairOneToManyData& data)
						{
							return data.owner_collider == owner_obj_collider;
						});
						if (!is_maked)
						{
							collider_pairs.emplace_back(owner_obj_collider, std::vector<TargetColliderData>());
						}

						// オーナーが同じデータへターゲットを追加
						for (int i = 0; i < collider_pairs.size(); ++i)
						{
							if (collider_pairs.at(i).owner_collider == owner_obj_collider)
							{
								collider_pairs.at(i).target_data.emplace_back(TargetColliderData(target_obj_collider, intersection));
							}
						}
					}
				}
			}
		}
	}

	return collider_pairs;
}


#pragma region 衝突判定
bool CollisionManager::IsHit					(const Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = owner_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return IsHitModelAndTarget(owner_collider.GetModelHandle(), target_collider, intersection);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kLine:			return IsHitLineAndTarget		(*std::dynamic_pointer_cast<Line>		(shape), target_collider, intersection); break;
	case ShapeKind::kSegment:		return IsHitSegmentAndTarget	(*std::dynamic_pointer_cast<Segment>	(shape), target_collider, intersection); break;
	case ShapeKind::kPlane:			return IsHitPlaneAndTarget		(*std::dynamic_pointer_cast<Plane>		(shape), target_collider, intersection); break;
	case ShapeKind::kTriangle:		return IsHitTriangleAndTarget	(*std::dynamic_pointer_cast<Triangle>	(shape), target_collider, intersection); break;
	case ShapeKind::kSquare:		return IsHitSquareAndTarget		(*std::dynamic_pointer_cast<Square>		(shape), target_collider, intersection); break;
	case ShapeKind::kAABB:			return IsHitAABBAndTarget		(*std::dynamic_pointer_cast<AABB>		(shape), target_collider, intersection); break;
	case ShapeKind::kOBB:			return IsHitOBBAndTarget		(*std::dynamic_pointer_cast<OBB>		(shape), target_collider, intersection); break;
	case ShapeKind::kSphere:		return IsHitSphereAndTarget		(*std::dynamic_pointer_cast<Sphere>		(shape), target_collider, intersection); break;
	case ShapeKind::kCapsule:		return IsHitCapsuleAndTarget	(*std::dynamic_pointer_cast<Capsule>	(shape), target_collider, intersection); break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitLineAndTarget		(const Line&	 line,			 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = target_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return false;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kLine:	return collision::IsHitLineAndLine (line, *std::dynamic_pointer_cast<Line> (shape), intersection);	break;
	case ShapeKind::kPlane:	return collision::IsHitLineAndPlane(line, *std::dynamic_pointer_cast<Plane>(shape), intersection);	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitSegmentAndTarget	(const Segment&  segment,		 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = target_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return collision::IsHitSegmentAndModel(segment, target_collider.GetModelHandle(), intersection);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kSegment:	return collision::IsHitSegmentAndSegment (segment, *std::dynamic_pointer_cast<Segment>	(shape), intersection);	break;
	case ShapeKind::kPlane:		return collision::IsHitSegmentAndPlane	 (segment, *std::dynamic_pointer_cast<Plane>	(shape), intersection);	break;
	case ShapeKind::kTriangle:	return collision::IsHitSegmentAndTriangle(segment, *std::dynamic_pointer_cast<Triangle>	(shape), intersection);	break;
	case ShapeKind::kSquare:	return collision::IsHitSegmentAndSquare	 (segment, *std::dynamic_pointer_cast<Square>	(shape), intersection);	break;
	case ShapeKind::kCapsule:	return collision::IsHitSegmentAndCapsule (segment, *std::dynamic_pointer_cast<Capsule>	(shape), intersection);	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitPlaneAndTarget		(const Plane&	 plane,			 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = target_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return false;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitPlaneAndCapsule(plane, *std::dynamic_pointer_cast<Capsule> (shape), intersection);	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitTriangleAndTarget	(const Triangle& triangle,		 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = target_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return collision::IsHitTriangleAndModel(triangle, target_collider.GetModelHandle(), intersection);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitTriangleAndCapsule(triangle, *std::dynamic_pointer_cast<Capsule> (shape), intersection);	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitSquareAndTarget		(const Square&	 square,		 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = target_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return false;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitSquareAndCapsule(square, *std::dynamic_pointer_cast<Capsule> (shape), intersection);	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitAABBAndTarget		(const AABB&	 aabb,			 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}

bool CollisionManager::IsHitOBBAndTarget		(const OBB&		 obb,			 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}

bool CollisionManager::IsHitSphereAndTarget		(const Sphere&	 sphere,		 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = target_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return collision::IsHitSphereAndModel(sphere, target_collider.GetModelHandle(), intersection);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kSphere:	return collision::IsHitSphereAndSphere (sphere, *std::dynamic_pointer_cast<Sphere> (shape), intersection);	break;
	case ShapeKind::kCapsule:	return collision::IsHitSphereAndCapsule(sphere, *std::dynamic_pointer_cast<Capsule>(shape), intersection);	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitCapsuleAndTarget	(const Capsule&  capsule,		 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = target_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return collision::IsHitCapsuleAndModel(capsule, target_collider.GetModelHandle(), intersection);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:	return collision::IsHitCapsuleAndCapsule(capsule, *std::dynamic_pointer_cast<Capsule> (shape), intersection);	break;

	default: break;
	}
	return false;
}

bool CollisionManager::IsHitModelAndTarget		(const int		 model_handle,	 const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}
#pragma endregion
