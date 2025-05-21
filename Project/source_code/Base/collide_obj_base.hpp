#pragma once
#include <cassert>

#include "../Base/obj_base.hpp"
#include "../Base/shape_base.hpp"
#include "../Data/Kind/trigger_kind.hpp"
#include "../Data/Kind/mass_step_kind.hpp"

/// @brief 衝突判定を行うオブジェクト
class CollideObjBase abstract : public ObjBase
{
public:
	CollideObjBase(const std::string& name, const std::string& tag) : 
		ObjBase				(name, tag),
		m_collider			(nullptr)
	{ }

	virtual ~CollideObjBase() = default;

	virtual void OnCollide(const CollideObjBase& check_hit_obj)abstract;

	/// @brief コライダーを作成(作成済みの場合はエラー)
	void MakeCollider(std::shared_ptr<ShapeBase> collider)
	{
		assert(m_collider == nullptr);
		m_collider = collider;
	}

	void AddTrigger(const TriggerKind trigger_kind, std::shared_ptr<ShapeBase> trigger) { m_trigger[trigger_kind]	= trigger; }

	[[nodiscard]] const std::shared_ptr<ShapeBase> GetCollider()const { return m_collider; }
	[[nodiscard]] const std::shared_ptr<ShapeBase> GetTrigger(const TriggerKind trigger_kind)const
	{
		if (m_trigger.count(trigger_kind))
		{
			return m_trigger.at(trigger_kind);
		}
		return nullptr;
	}

protected:
	std::shared_ptr<ShapeBase> m_collider;
	std::unordered_map<TriggerKind, std::shared_ptr<ShapeBase>> m_trigger;
	MassStepKind m_mass_step_kind;
};
