#pragma once
#include "../Base/singleton_base.hpp"
#include "../Base/physical_obj_base.hpp"

class PhysicsManager final : public SingletonBase<PhysicsManager>
{
public:
	void Update();

	/// @brief 衝突判定を行うオブジェクトを追加
	void AddPhysicalObj(std::shared_ptr<PhysicalObjBase> collide_obj);
	/// @brief 衝突判定を行うオブジェクトを解除
	void RemovePhysicalObj(std::shared_ptr<PhysicalObjBase> collide_obj);

private:
	PhysicsManager();
	~PhysicsManager() override;

private:
	friend SingletonBase<PhysicsManager>;
};
