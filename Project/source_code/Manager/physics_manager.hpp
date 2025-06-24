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

		// 無視リストに登録されている、もしくは静的オブジェクトであった場合は重力を適用させない
		if (itr != m_ignore_gravity_obj_name.end() || physical_obj->GetMassKind() == MassKind::kStatic)
		{
			return false;
		}

		return true;
	}

private:
	PhysicsManager();
	~PhysicsManager() override;

	/// @brief すべてのオブジェクトの押し戻し処理を行う
	void ExecutePushBackPairs();

	#pragma region 押し戻し
	void PushBack				 (const std::shared_ptr<PhysicalObjBase> low_priority_obj, const std::shared_ptr<PhysicalObjBase> high_priority_obj);
	void PushBackSphereAndTarget (const std::shared_ptr<PhysicalObjBase> low_priority_obj, const std::shared_ptr<PhysicalObjBase> high_priority_obj);
	void PushBackCapsuleAndTarget(const std::shared_ptr<PhysicalObjBase> low_priority_obj, const std::shared_ptr<PhysicalObjBase> high_priority_obj);
	#pragma endregion


private:
	static constexpr float kGravityAcceleration = 0.98f;				// 重力加速度(デルタタイム適用前)
	static constexpr float kMaxGravity			= 0.2f;					// 最大重力(デルタタイム適用後の最大値)

	std::vector<std::shared_ptr<PhysicalObjBase>> m_physical_objects;	// 物理的挙動を行うオブジェクト
	std::vector<std::string> m_ignore_physical_behavior_obj_name;		// 物理的な挙動全般を無視するオブジェクト
	std::vector<std::string> m_ignore_gravity_obj_name;					// 重力の影響を無視するオブジェクト

	friend SingletonBase<PhysicsManager>;
};
