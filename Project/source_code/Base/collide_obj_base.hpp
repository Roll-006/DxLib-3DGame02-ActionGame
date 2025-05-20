#pragma once
#include "../Base/obj_base.hpp"
#include "../Base/shape_base.hpp"
#include "../Data/Kind/trigger_kind.hpp"

/// @brief 衝突判定を行うオブジェクト
class CollideObjBase : public ObjBase
{
public:
	CollideObjBase(const std::string& name, const std::string& tag) : 
		ObjBase				(name, tag),
		m_collider			(nullptr),
		m_landing_trigger	(nullptr)
	{ }

	virtual ~CollideObjBase() = default;

	virtual void OnCollide(const CollideObjBase& check_hit_obj) = 0;

	void MakeCollider		(std::shared_ptr<ShapeBase> collider)						{ m_collider				= collider; }
	void MakeLandingTrigger	(std::shared_ptr<Sphere>	landing_trigger)				{ m_landing_trigger			= landing_trigger; }
	void AddTrigger(const TriggerKind trigger_kind, std::shared_ptr<ShapeBase> trigger) { m_trigger[trigger_kind]	= trigger; }

	[[nodiscard]] const std::shared_ptr<ShapeBase>	GetCollider		 ()const { return m_collider; }
	[[nodiscard]] const std::shared_ptr<Sphere>		GetLandingTrigger()const { return m_landing_trigger; }
	[[nodiscard]] const std::shared_ptr<ShapeBase>	GetTrigger(const TriggerKind trigger_kind)const
	{
		if (m_trigger.count(trigger_kind))
		{
			return m_trigger.at(trigger_kind);
		}
		return nullptr;
	}

protected:
	std::shared_ptr<ShapeBase>	m_collider;									// コライダー
	std::shared_ptr<Sphere>		m_landing_trigger;							// 着地判定用トリガー
	std::unordered_map<TriggerKind, std::shared_ptr<ShapeBase>> m_trigger;	// 各トリガー
};
