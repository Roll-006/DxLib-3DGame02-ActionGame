#pragma once
#include <cassert>

#include "../Base/obj_base.hpp"
#include "../Data/IncludeList/shape.hpp"
#include "../Data/Kind/trigger_kind.hpp"
#include "../Data/Kind/mass_kind.hpp"

/// @brief 物理的な挙動を行うオブジェクトの基底クラス
class PhysicalObjBase abstract : public ObjBase
{
public:
	PhysicalObjBase(const std::string& name, const std::string& tag, MassKind mass_level_kind) :
		ObjBase		(name, tag),
		m_collider	(nullptr),
		m_mass_kind	(mass_level_kind)
	{ }
	
	virtual ~PhysicalObjBase() = default;

	virtual void OnCollide(const PhysicalObjBase& check_hit_obj)abstract;
	virtual void OnGravity()abstract;

	[[nodiscard]] const std::shared_ptr<ShapeBase> GetCollider() const { return m_collider; }
	[[nodiscard]] const std::shared_ptr<ShapeBase> GetTrigger(const TriggerKind trigger_kind) const
	{
		if (m_trigger.count(trigger_kind))
		{
			return m_trigger.at(trigger_kind);
		}
		return nullptr;
	}
	[[nodiscard]] MassKind GetMassKind()const { return m_mass_kind; }

protected:
	/// @brief コライダーを作成(作成済みの場合はエラー)
	void MakeCollider(std::shared_ptr<ShapeBase> collider)
	{
		assert(m_collider == nullptr);
		m_collider = collider;
	}

	void AddTrigger(const TriggerKind trigger_kind, std::shared_ptr<ShapeBase> trigger)
	{
		if (!m_trigger.count(trigger_kind))
		{
			m_trigger[trigger_kind] = trigger;
		}
	}

protected:
	MassKind m_mass_kind;
	std::shared_ptr<ShapeBase> m_collider;
	std::unordered_map<TriggerKind, std::shared_ptr<ShapeBase>> m_trigger;
};
