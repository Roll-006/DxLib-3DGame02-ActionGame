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

void CollisionManager::Update()
{
	for (const auto& obj : m_collide_objects)
	{
		obj->ReleaseLanding();
		obj->RemoveHitTriangles();
		obj->RemoveHitCollider();
	}

	const auto collider_pairs = CreateHitColliderPairs();

	// 衝突したコライダーのオーナーオブジェクトの処理を実行
	for (const auto& pair : collider_pairs)
	{
		// レイキャストトリガーの場合は最初に衝突したコライダーのみ衝突したものとする
		if (pair.owner_collider->GetColliderKind() == ColliderKind::kRayCast && pair.owner_collider->IsClosestOnlyHit())
		{
			// コライダーとの距離を取得
			std::unordered_map<int, TargetColliderData> target;
			std::vector<std::pair<int, float>>			distance;
			for (size_t i = 0; i < pair.target_data.size(); ++i)
			{
				if (pair.target_data.at(i).intersection)
				{
					target[i] = TargetColliderData(pair.target_data.at(i).collider, pair.target_data.at(i).intersection);

					const auto dist = VSize(*pair.target_data.at(i).intersection - std::static_pointer_cast<Segment>(pair.owner_collider->GetShape())->GetBeginPos());
					distance.emplace_back(std::make_pair(i, dist));
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
			if (!IsApplyCollide(owner_obj->GetName(), owner_obj_collider.first)) { continue; }

			for (const auto& target_obj : m_collide_objects)
			{
				// 自身との当たり判定は避ける
				if (owner_obj == target_obj) { continue; }

				// 非アクティブの場合はスキップ
				if (!target_obj->IsActive()) { continue; }

				for (const auto& target_obj_collider : target_obj->GetColliderAll())
				{
					// 衝突が許可されている場合のみ処理を続行
					if (!IsApplyCollide(target_obj->GetName(), target_obj_collider.first)) { continue; }

					// 衝突判定
					std::optional<VECTOR> intersection;
					if (IsHit(*owner_obj_collider.second, *target_obj_collider.second, intersection))
					{
						// 指定のオーナーのデータコンテナがまだない場合は新たに作成
						bool is_maked = std::any_of(collider_pairs.begin(), collider_pairs.end(), [=](const ColliderPairOneToManyData& data)
						{
							return data.owner_collider == owner_obj_collider.second;
						});
						if (!is_maked)
						{
							collider_pairs.emplace_back(owner_obj_collider.second, std::vector<TargetColliderData>());
						}

						// オーナーが同じデータへターゲットを追加
						for (size_t i = 0; i < collider_pairs.size(); ++i)
						{
							if (collider_pairs.at(i).owner_collider == owner_obj_collider.second)
							{
								collider_pairs.at(i).target_data.emplace_back(TargetColliderData(target_obj_collider.second, intersection));
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
bool CollisionManager::IsHit					(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto shape = owner_collider.GetShape();

	// 図形の登録がされていない場合はモデルで判定を行う
	if (shape == nullptr)
	{
		return IsHitModelAndTarget(owner_collider, target_collider, intersection);
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kLine:		return IsHitLineAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kSegment:	return IsHitSegmentAndTarget	(owner_collider, target_collider, intersection);
	case ShapeKind::kPlane:		return IsHitPlaneAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kTriangle:	return IsHitTriangleAndTarget	(owner_collider, target_collider, intersection);
	case ShapeKind::kSquare:	return IsHitSquareAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kAABB:		return IsHitAABBAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kOBB:		return IsHitOBBAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kSphere:	return IsHitSphereAndTarget		(owner_collider, target_collider, intersection);
	case ShapeKind::kCapsule:	return IsHitCapsuleAndTarget	(owner_collider, target_collider, intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsHitLineAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Line>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		return false;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kLine:
		return collision::IsHitLineAndLine (owner_shape, *std::static_pointer_cast<Line> (target_shape), intersection);

	case ShapeKind::kPlane:
		return collision::IsHitLineAndPlane(owner_shape, *std::static_pointer_cast<Plane>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsHitSegmentAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Segment>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		std::vector<Triangle> hit_triangles;
		const bool is_hit = collision::IsHitSegmentAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}

		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kSegment:
		return collision::IsHitSegmentAndSegment (owner_shape, *std::static_pointer_cast<Segment> (target_shape), intersection);

	case ShapeKind::kPlane:
		return collision::IsHitSegmentAndPlane	 (owner_shape, *std::static_pointer_cast<Plane>   (target_shape), intersection);

	case ShapeKind::kTriangle:
		return collision::IsHitSegmentAndTriangle(owner_shape, *std::static_pointer_cast<Triangle>(target_shape), intersection);

	case ShapeKind::kSquare:
		return collision::IsHitSegmentAndSquare	 (owner_shape, *std::static_pointer_cast<Square>  (target_shape), intersection);

	case ShapeKind::kCapsule:
		return collision::IsHitSegmentAndCapsule (owner_shape, *std::static_pointer_cast<Capsule> (target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsHitPlaneAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Plane>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		return false;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		return collision::IsHitPlaneAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsHitTriangleAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Triangle>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		std::vector<Triangle> hit_triangles;
		const bool is_hit = collision::IsHitTriangleAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}

		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		return collision::IsHitTriangleAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsHitSquareAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Square>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		return false;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		return collision::IsHitSquareAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsHitAABBAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}

bool CollisionManager::IsHitOBBAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}

bool CollisionManager::IsHitSphereAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape	= target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Sphere>(owner_collider.GetShape());
	bool	   is_hit		= false;
	std::vector<Triangle> hit_triangles;

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		is_hit = collision::IsHitSphereAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}

		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kTriangle:
		is_hit = collision::IsHitTriangleAndSphere(*std::static_pointer_cast<Triangle>(target_shape), owner_shape, intersection);

		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}
		break;

	case ShapeKind::kSphere:
		return collision::IsHitSphereAndSphere (owner_shape, *std::dynamic_pointer_cast<Sphere>(target_shape), intersection);
	
	case ShapeKind::kCapsule:
		return collision::IsHitSphereAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return is_hit;
}

bool CollisionManager::IsHitCapsuleAndTarget	(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	const auto target_shape = target_collider.GetShape();
	const auto owner_shape	= *std::static_pointer_cast<Capsule>(owner_collider.GetShape());

	// 図形の登録がされていない場合はモデルで判定を行う
	if (target_shape == nullptr)
	{
		std::vector<Triangle> hit_triangles;
		const bool is_hit = collision::IsHitCapsuleAndModel(owner_shape, target_collider.GetModelHandle(), intersection, hit_triangles);
		
		// 衝突対象がコライダーであった場合は三角形情報を追加する
		if (is_hit && target_collider.GetColliderKind() == ColliderKind::kCollider)
		{
			owner_collider.AddHitModelTriangle(target_collider.GetModelHandle(), hit_triangles);
		}
		return is_hit;
	}

	switch (target_shape->GetShapeKind())
	{
	case ShapeKind::kSegment:
		return collision::IsHitSegmentAndCapsule(*std::dynamic_pointer_cast<Segment>(target_shape), owner_shape, intersection);

	case ShapeKind::kCapsule:
		return collision::IsHitCapsuleAndCapsule(owner_shape, *std::static_pointer_cast<Capsule>(target_shape), intersection);

	default:
		break;
	}

	return false;
}

bool CollisionManager::IsHitModelAndTarget		(Collider& owner_collider, const Collider& target_collider, std::optional<VECTOR>& intersection)
{
	return false;
}
#pragma endregion
