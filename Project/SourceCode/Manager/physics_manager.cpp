#include "physics_manager.hpp"

PhysicsManager::PhysicsManager()
{
	// 処理なし
}

PhysicsManager::~PhysicsManager()
{
	// 処理なし
}

void PhysicsManager::Update()
{
	for (const auto& obj : m_physical_objects)
	{
		// 重力処理が許可されている場合のみ重力を与える
		if (IsApplyGravity(obj) && obj->IsActive())
		{
			obj->ApplyGravity(kGravityAcceleration, kMaxGravity);		
		}
	}
}

void PhysicsManager::LateUpdate()
{
	for (const auto& obj : m_physical_objects)
	{
		if (IsApplyGravity(obj) && obj->IsActive())
		{
			obj->AddFallVelocity();

			// velocityが浮かないよう張り付ける
			obj->ProjectionVelocity();
		}
	}

	// 押し戻し(有効な速度ベクトルを取得)
	ExecutePushBackPairs();

	for (const auto& obj : m_physical_objects)
	{
		if (std::isnan(obj->GetVelocity().x))
		{
			int a = 0;
		}

		// velocityをオブジェクトに適用
		obj->ApplyVelocity();

		if (std::isnan(obj->GetVelocity().x))
		{
			int a = 0;
		}
	}

	for (const auto& obj : m_physical_objects)
	{
		ProjectionPos(obj);
	}
}


#pragma region 登録・解除
void PhysicsManager::RemovePhysicalObj				(const int obj_handle)
{
	const auto physical_obj = ObjManager::GetInstance()->GetObj<PhysicalObjBase>(obj_handle);

	if (std::find(m_physical_objects.begin(), m_physical_objects.end(), physical_obj) != m_physical_objects.end())
	{
		erase(m_physical_objects, physical_obj);
	}
}

void PhysicsManager::AddIgnoreObjPhysicalBehavior   (const int obj_handle)
{
	if (std::find(m_ignore_physical_behavior_obj_handle.begin(), m_ignore_physical_behavior_obj_handle.end(), obj_handle) == m_ignore_physical_behavior_obj_handle.end())
	{
		m_ignore_physical_behavior_obj_handle.emplace_back(obj_handle);
	}
}

void PhysicsManager::RemoveIgnoreObjPhysicalBehavior(const int obj_handle)
{
	if (std::find(m_ignore_physical_behavior_obj_handle.begin(), m_ignore_physical_behavior_obj_handle.end(), obj_handle) != m_ignore_physical_behavior_obj_handle.end())
	{
		erase(m_ignore_physical_behavior_obj_handle, obj_handle);
	}
}

void PhysicsManager::AddIgnoreObjGravity			(const int obj_handle)
{
	if (std::find(m_ignore_gravity_obj_handle.begin(), m_ignore_gravity_obj_handle.end(), obj_handle) == m_ignore_gravity_obj_handle.end())
	{
		m_ignore_gravity_obj_handle.emplace_back(obj_handle);
	}
}

void PhysicsManager::RemoveIgnoreObjGravity			(const int obj_handle)
{
	if (std::find(m_ignore_gravity_obj_handle.begin(), m_ignore_gravity_obj_handle.end(), obj_handle) != m_ignore_gravity_obj_handle.end())
	{
		erase(m_ignore_gravity_obj_handle, obj_handle);
	}
}
#pragma endregion


void PhysicsManager::ProjectionPos(const std::shared_ptr<PhysicalObjBase> physical_obj)
{
	// FIXME : 張り付け押し戻し作成中
	//const auto collider_shape = physical_obj->GetCollider(ColliderKind::kCollider)->GetShape();

	//if (!physical_obj->IsActive())								{ return; }
	//if (!collider_shape)										{ return; }
	//if (collider_shape->GetShapeKind() != ShapeKind::kCapsule)	{ return; }
	//
	//for (const auto& target_obj : m_physical_objects)
	//{
	//	// 自身との当たり判定は避ける
	//	if (physical_obj == target_obj) { continue; }
	//
	//	// 非アクティブであれば適用しない
	//	if (!target_obj	 ->IsActive()) { continue; }
	//
	//	// 衝突が許可されていない場合は以降の処理をスキップ
	//	if (!IsApplyPhysicalBehavior(target_obj)) { continue; }
	//
	//	const auto target_collider	= target_obj->GetCollider(ColliderKind::kCollider);
	//	const auto target_shape		= target_collider->GetShape();
	//	MV1_COLL_RESULT_POLY_DIM hit_result;
	//
	//	if (!target_shape)
	//	{
	//		collision::IsHitCapsuleAndModel(*std::dynamic_pointer_cast<Capsule>(collider_shape), target_collider->GetModelHandle(), hit_result);
	//	}
	//
	//}
}

void PhysicsManager::ExecutePushBackPairs()
{
	// オブジェクトが持つコライダー、もしくはメッシュの押し戻し処理を行う
	// TODO : 後に軽量化
	for (const auto& obj_1 : m_physical_objects)
	{
		// 非アクティブであれば適用しない
		if (!obj_1->IsActive()) { continue; }

		// 衝突が許可されていない場合は以降の処理をスキップ
		if (!IsApplyPhysicalBehavior(obj_1)) { continue; }
		if (obj_1->GetCollider(ColliderKind::kCollider) == nullptr) { continue; }

		for (const auto& obj_2 : m_physical_objects)
		{
			// 自身との当たり判定は避ける
			if (obj_1 == obj_2) { continue; }

			// 非アクティブであれば適用しない
			if (!obj_2->IsActive()) { continue; }

			// 衝突が許可されていない場合は以降の処理をスキップ
			if (!IsApplyPhysicalBehavior(obj_2)) { continue; }
			if (obj_2->GetCollider(ColliderKind::kCollider) == nullptr) { continue; }

			// 互いに静的オブジェクトであった場合は以降の処理をスキップ
			if (obj_1->GetMassKind() == MassKind::kStatic && obj_2->GetMassKind() == MassKind::kStatic) { continue; }

			// 質量を考慮して押し戻される側を判定
			std::shared_ptr<PhysicalObjBase> low_priority_obj  = obj_1;
			std::shared_ptr<PhysicalObjBase> high_priority_obj = obj_2;
			if (low_priority_obj->GetMassKind() > high_priority_obj->GetMassKind())
			{
				low_priority_obj  = obj_2;
				high_priority_obj = obj_1;
			}

			if (low_priority_obj->GetName() == ObjName.PLAYER && high_priority_obj->GetName() == ObjName.GROUND)
			{
				int a = 0;
			}

			// 押し戻し処理を実行
			PushBack(low_priority_obj, high_priority_obj);
		}
	}
}


#pragma region 押し戻し
void PhysicsManager::PushBack(const std::shared_ptr<PhysicalObjBase> low_priority_obj, const std::shared_ptr<PhysicalObjBase> high_priority_obj)
{
	const auto* shape = low_priority_obj->GetCollider(ColliderKind::kCollider)->GetShape().get();

	// 図形の登録がされていない場合はモデルで押し戻しを行う
	if (shape == nullptr)
	{
		return;
	}

	switch (shape->GetShapeKind())
	{
	//case ShapeKind::kTriangle:  PushBackTriangleAndTarget(low_priority_obj, high_priority_obj);  break;
	case ShapeKind::kSphere:	PushBackSphereAndTarget  (low_priority_obj, high_priority_obj);  break;
	case ShapeKind::kCapsule:	PushBackCapsuleAndTarget (low_priority_obj, high_priority_obj);  break;

	default: break;
	}
}

void PhysicsManager::PushBackTriangleAndTarget(const std::shared_ptr<PhysicalObjBase> low_priority_obj, const std::shared_ptr<PhysicalObjBase> high_priority_obj)
{
	const auto* shape		= high_priority_obj->GetCollider(ColliderKind::kCollider)->GetShape().get();
	const auto  velocity	= low_priority_obj->GetVelocity();
	const auto  triangle	= *std::dynamic_pointer_cast<Triangle>(low_priority_obj->GetCollider(ColliderKind::kCollider)->GetShape());

	// 図形の登録がされていない場合はモデルで押し戻しを行う
	if (shape == nullptr)
	{
		return;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kCapsule:
		const auto push_backed_velocity = collision::PushBackCapsuleAndTriangle(velocity, *static_cast<const Capsule*>(shape), triangle, kSlopeDifficultyAngleThreshold, kMaxSlopeAngle);
		low_priority_obj->SetVelocity(push_backed_velocity);
		break;

	default:
		break;
	}
}

void PhysicsManager::PushBackSphereAndTarget (const std::shared_ptr<PhysicalObjBase> low_priority_obj, const std::shared_ptr<PhysicalObjBase> high_priority_obj)
{
	const auto* shape		= high_priority_obj->GetCollider(ColliderKind::kCollider)->GetShape().get();
	const auto  velocity	= low_priority_obj->GetVelocity();
	const auto  sphere		= *std::dynamic_pointer_cast<Sphere>(low_priority_obj->GetCollider(ColliderKind::kCollider)->GetShape());

	// 図形の登録がされていない場合はモデルで押し戻しを行う
	if (shape == nullptr)
	{
		const auto model_handle			= high_priority_obj->GetColliderModelHandle();
		const auto push_backed_velocity = collision::PushBackSphereAndModel(velocity, sphere, model_handle, kSlopeDifficultyAngleThreshold, kMaxSlopeAngle);
		low_priority_obj->SetVelocity(push_backed_velocity);
		return;
	}

	switch (shape->GetShapeKind())
	{
	default:
		break;
	}
}

void PhysicsManager::PushBackCapsuleAndTarget(const std::shared_ptr<PhysicalObjBase> low_priority_obj, const std::shared_ptr<PhysicalObjBase> high_priority_obj)
{
	const auto* shape		= high_priority_obj->GetCollider(ColliderKind::kCollider)->GetShape().get();
	const auto  velocity	= low_priority_obj->GetVelocity();
	const auto  capsule		= *std::dynamic_pointer_cast<Capsule>(low_priority_obj->GetCollider(ColliderKind::kCollider)->GetShape());
	auto push_backed_velocity = v3d::GetZeroV();

	// 図形の登録がされていない場合はモデルで押し戻しを行う
	if (shape == nullptr)
	{
		const auto model_handle	= high_priority_obj->GetColliderModelHandle();
		push_backed_velocity	= collision::PushBackCapsuleAndModel(velocity, capsule, model_handle, kSlopeDifficultyAngleThreshold, kMaxSlopeAngle);
		low_priority_obj->SetVelocity(push_backed_velocity);
		return;
	}

	switch (shape->GetShapeKind())
	{
	case ShapeKind::kTriangle:
		push_backed_velocity = collision::PushBackCapsuleAndTriangle(velocity, capsule, *static_cast<const Triangle*>(shape), kSlopeDifficultyAngleThreshold, kMaxSlopeAngle);
		low_priority_obj->SetVelocity(push_backed_velocity);
		break;

	default:
		break;
	}
}
#pragma endregion
