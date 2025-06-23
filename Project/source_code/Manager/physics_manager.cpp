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
		if (IsApplyGravity(obj))
		{
			obj->ApplyGravity(kGravityAcceleration, kMaxGravity);
		}
	}
}

void PhysicsManager::LateUpdate()
{
	for (const auto& obj : m_physical_objects)
	{
		// 落下ベクトルを速度ベクトルに適用
		obj->AddFallVelocity();
	}

	// 押し戻し(有効な速度ベクトルを取得)
	PushBackAllPhysicalObj();

	for (const auto& obj : m_physical_objects)
	{
		// 速度ベクトルを座標・コライダーに適用
		obj->ApplyVelocityToPos();
		obj->ApplyVelocityToCollider();
	}
}


#pragma region 登録・解除
void PhysicsManager::RemovePhysicalObj				(const std::string& obj_name)
{
	const auto physical_obj = std::static_pointer_cast<PhysicalObjBase>(ObjManager::GetInstance()->GetObj(obj_name));

	if (std::find(m_physical_objects.begin(), m_physical_objects.end(), physical_obj) != m_physical_objects.end())
	{
		erase(m_physical_objects, physical_obj);
	}
}

void PhysicsManager::AddIgnoreObjPhysicalBehavior   (const std::string& obj_name)
{
	if (std::find(m_ignore_physical_behavior_obj_name.begin(), m_ignore_physical_behavior_obj_name.end(), obj_name) == m_ignore_physical_behavior_obj_name.end())
	{
		m_ignore_physical_behavior_obj_name.emplace_back(obj_name);
	}
}

void PhysicsManager::RemoveIgnoreObjPhysicalBehavior(const std::string& obj_name)
{
	if (std::find(m_ignore_physical_behavior_obj_name.begin(), m_ignore_physical_behavior_obj_name.end(), obj_name) != m_ignore_physical_behavior_obj_name.end())
	{
		erase(m_ignore_physical_behavior_obj_name, obj_name);
	}
}

void PhysicsManager::AddIgnoreObjGravity			(const std::string& obj_name)
{
	if (std::find(m_ignore_gravity_obj_name.begin(), m_ignore_gravity_obj_name.end(), obj_name) == m_ignore_gravity_obj_name.end())
	{
		m_ignore_gravity_obj_name.emplace_back(obj_name);
	}
}

void PhysicsManager::RemoveIgnoreObjGravity			(const std::string& obj_name)
{
	if (std::find(m_ignore_gravity_obj_name.begin(), m_ignore_gravity_obj_name.end(), obj_name) != m_ignore_gravity_obj_name.end())
	{
		erase(m_ignore_gravity_obj_name, obj_name);
	}
}
#pragma endregion


#pragma region 押し戻し
void PhysicsManager::PushBackAllPhysicalObj()
{
	// オブジェクトが持つコライダー、もしくはメッシュの押し戻し処理を行う
	// TODO : 後に軽量化
	for (const auto& obj_1 : m_physical_objects)
	{
		// 衝突が許可されている場合のみ処理を続行
		if (!IsApplyPhysicalBehavior(obj_1)) { continue; }

		for (const auto& obj_2 : m_physical_objects)
		{
			// 自身との当たり判定は避ける
			if (obj_1 == obj_2) { continue; }

			// 衝突が許可されている場合のみ処理を続行
			if (!IsApplyPhysicalBehavior(obj_2)) { continue; }

			// 質量を考慮して押し戻される側を判定
			std::vector<std::shared_ptr<PhysicalObjBase>> obj { obj_1, obj_2 };
			if (obj.at(0)->GetMassKind() > obj.at(1)->GetMassKind())
			{
				obj.at(0) = obj_2;
				obj.at(1) = obj_1;
			}








			//if (IsHit(*owner_obj_collider, *target_obj_collider))
			//{
			//	collider_pair.emplace_back(ColliderPairData(owner_obj_collider, target_obj_collider));
			//	collider_pair.emplace_back(ColliderPairData(owner_obj_collider, target_obj_collider));
			//}
		}
	}
}
#pragma endregion
