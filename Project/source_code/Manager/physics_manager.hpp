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
	void AddIgnoreObjPhysical   (const std::string& obj_name);
	/// @brief 物理的な挙動全般を無視するオブジェクトから除外
	void RemoveIgnoreObjPhysical(const std::string& obj_name);

	/// @brief 重力の影響を無視するオブジェクトを追加
	void AddIgnoreObjGravity   (const std::string& obj_name);
	/// @brief 重力の影響を無視するオブジェクトから除外
	void RemoveIgnoreObjGravity(const std::string& obj_name);

	/// @brief 重力を適用するかを判定する
	[[nodiscard]] bool IsApplyGravity(const obj_concepts::PhysicalObjT auto& physical_obj) const
	{
		const auto itr = std::find(m_ignore_gravity_obj_name.begin(), m_ignore_gravity_obj_name.end(), physical_obj.GetName());
		return itr == m_ignore_gravity_obj_name.end();
	}

private:
	PhysicsManager();
	~PhysicsManager() override;

private:
	static constexpr float kGravityAcceleration = 1.0f;					// 重力加速度
	static constexpr float kMaxGravity			= 10.0f;				// 最大重力

	std::vector<std::shared_ptr<PhysicalObjBase>> m_physical_objects;	// 物理的挙動を行うオブジェクト
	std::vector<std::string> m_ignore_physical_obj_name;				// 物理的な挙動全般を無視するオブジェクト
	std::vector<std::string> m_ignore_gravity_obj_name;					// 重力の影響を無視するオブジェクト

	friend SingletonBase<PhysicsManager>;
};
