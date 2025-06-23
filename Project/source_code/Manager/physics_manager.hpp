#pragma once
#include "../Base/singleton_base.hpp"
#include "../Base/physical_obj_base.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Calculation/collision_calculator.hpp"

class PhysicsManager final : public SingletonBase<PhysicsManager>
{
public:
	void Update();
	void LateUpdate();


	// TODO : 仮
	void PushBack(PhysicalObjBase* physical_obj)
	{
		for (const auto& obj : m_physical_objects)
		{
			if (physical_obj == obj.get())		{ continue; }	// 自分自身との衝突はスキップ
			if (!IsApplyPhysicalBehavior(obj))	{ continue; }	// 物理的処理が無効な場合はスキップ

			if (physical_obj->GetName() != ObjName.PLAYER) { continue; }
			if (obj->GetName() != ObjName.GROUND) { continue; }

			const int  model_handle    = obj->GetModeler()->GetModelHandle();
			const auto current_capsule = *dynamic_cast<Capsule*>(physical_obj->GetCollider(ColliderKind::kCollider)->GetShape().get());
			Capsule future_capsule     = current_capsule;
			future_capsule.Move(physical_obj->GetVelocity());
			const auto segment         = future_capsule.GetSegment();
			const auto result          = MV1CollCheck_Capsule(model_handle, -1, segment.GetBeginPos(), segment.GetEndPos(), future_capsule.GetRadius());

			// 衝突が発生しなかった場合は移行の処理をスキップ
			if (!result.HitNum) { continue; }

			std::unordered_map<int, Triangle> triangles;
			std::unordered_map<int, float>    current_distance;
			for (int i = 0; i < result.HitNum; ++i)
			{
				Triangle triangle = Triangle(result.Dim[i].Position[0], result.Dim[i].Position[2], result.Dim[i].Position[1]);

				current_distance[i] = math::GetDistanceTriangleToCapsule(triangle, current_capsule);
				triangles[i] = triangle;
			}

			current_distance = math::Sort<int, float>(current_distance, SortKind::kAscending);

			for (const auto& distance : current_distance)
			{
				physical_obj->m_velocity = collision::GetValidVelocityAfterHitCapsuleAndTriangle(
					physical_obj->m_velocity, current_capsule, triangles.at(distance.first));
			}




			for (const auto& triangle : triangles)
			{
				triangle.second.Draw(true, 255, 0xffffff);
				DrawLine3D(triangle.second.GetCentroid(), triangle.second.GetCentroid() + triangle.second.GetNormalVector() * 10.0f, 0xffffff);
			}

			DrawFormatString(0,  0, 0xffffff, "result.HitNum         : %d", result.HitNum);
			DrawFormatString(0, 20, 0xffffff, "segment.GetBeginPos() : %f, %f, %f", segment.GetBeginPos().x, segment.GetBeginPos().y, segment.GetBeginPos().z);
		}
	}


	#pragma region 登録・解除
	/// @brief 物理的挙動を行うオブジェクトを追加
	template<obj_concepts::PhysicalObjT PhysicalObjT>
	void AddPhysicalObj   (const std::shared_ptr<PhysicalObjT> physical_obj)
	{
		// 上書き不可
		if (std::find(m_physical_objects.begin(), m_physical_objects.end(), physical_obj) == m_physical_objects.end())
		{
			m_physical_objects.emplace_back(physical_obj);
		}
	}
	/// @brief 物理的挙動を行うオブジェクトから除外
	void RemovePhysicalObj(const std::string& obj_name);

	/// @brief 物理的な挙動全般を無視するオブジェクトを追加
	void AddIgnoreObjPhysicalBehavior   (const std::string& obj_name);
	/// @brief 物理的な挙動全般を無視するオブジェクトから除外
	void RemoveIgnoreObjPhysicalBehavior(const std::string& obj_name);

	/// @brief 重力の影響を無視するオブジェクトを追加
	void AddIgnoreObjGravity   (const std::string& obj_name);
	/// @brief 重力の影響を無視するオブジェクトから除外
	void RemoveIgnoreObjGravity(const std::string& obj_name);
	#pragma endregion


	/// @brief 物理的な挙動を適用するかを判定する
	template<obj_concepts::PhysicalObjT PhysicalObjT>
	[[nodiscard]] bool IsApplyPhysicalBehavior(const std::shared_ptr<PhysicalObjT> physical_obj) const
	{
		const auto itr = std::find(m_ignore_physical_behavior_obj_name.begin(), m_ignore_physical_behavior_obj_name.end(), physical_obj->GetName());
		return itr == m_ignore_physical_behavior_obj_name.end();
	}

	/// @brief 重力を適用するかを判定する
	template<obj_concepts::PhysicalObjT PhysicalObjT>
	[[nodiscard]] bool IsApplyGravity(const std::shared_ptr<PhysicalObjT> physical_obj) const
	{
		const auto itr = std::find(m_ignore_gravity_obj_name.begin(), m_ignore_gravity_obj_name.end(), physical_obj->GetName());

		if (itr == m_ignore_gravity_obj_name.end() || physical_obj->GetMassKind() == MassKind::kStatic)
		{
			return true;
		}
		return false;
	}

private:
	PhysicsManager();
	~PhysicsManager() override;

private:
	static constexpr float kGravityAcceleration = 1.0f;					// 重力加速度
	static constexpr float kMaxGravity			= 0.1f;					// 最大重力

	std::vector<std::shared_ptr<PhysicalObjBase>> m_physical_objects;	// 物理的挙動を行うオブジェクト
	std::vector<std::string> m_ignore_physical_behavior_obj_name;		// 物理的な挙動全般を無視するオブジェクト
	std::vector<std::string> m_ignore_gravity_obj_name;					// 重力の影響を無視するオブジェクト

	friend SingletonBase<PhysicsManager>;
};
