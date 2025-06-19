#pragma once
#include "../Base/singleton_base.hpp"
#include "../Base/physical_obj_base.hpp"

#include "../Manager/obj_manager.hpp"

class PhysicsManager final : public SingletonBase<PhysicsManager>
{
public:
	void Update();

	/// @brief 物理的挙動を行うオブジェクトを追加
	template<obj_concepts::PhysicalObjT PhysicalObjT>
	void AddPhysicalObj(const PhysicalObjT& physical_obj)
	{
		const auto obj = std::make_shared<PhysicalObjT>(physical_obj);

		// 上書き不可
		if (!std::count(m_physical_objects.begin(), m_physical_objects.end(), obj))
		{
			m_physical_objects.emplace_back(obj);
		}
	}
	/// @brief 物理的挙動を行うオブジェクトから除外
	void RemovePhysicalObj(std::string obj_name);

	/// @brief 物理的挙動を無視するオブジェクトを追加
	void AddIgnoreObj(std::string obj_name);
	/// @brief 物理的挙動を無視するオブジェクトから除外
	void RemoveIgnoreObj(std::string obj_name);

private:
	PhysicsManager();
	~PhysicsManager() override;

private:
	static constexpr float kGravityAcceleration = 1.0f;				// 重力加速度
	static constexpr float kMaxGravity			= 10.0f;			// 最大重力

	std::list<std::shared_ptr<PhysicalObjBase>> m_physical_objects;	// 物理的挙動を行うオブジェクト
	std::list<std::string> m_ignore_obj_name;						// 物理的挙動を無視するオブジェクト

	friend SingletonBase<PhysicsManager>;
};
