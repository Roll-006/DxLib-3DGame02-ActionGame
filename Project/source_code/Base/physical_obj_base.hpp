#pragma once
#include "../Base/obj_base.hpp"

#include "../Data/IncludeList/shape.hpp"
#include "../Data/Kind/trigger_kind.hpp"
#include "../Data/Kind/mass_kind.hpp"

#include "../FPS/fps.hpp"

/// @brief 物理的な挙動を行うオブジェクトの基底クラス
class PhysicalObjBase abstract : public ObjBase
{
public:
	PhysicalObjBase(const std::string& name, const std::string& tag, MassKind mass_level_kind) :
		ObjBase			(name, tag),
		m_fall_speed	(0.0f),
		m_fall_velocity	(v3d::GetZeroV()),
		m_velocity		(v3d::GetZeroV()),
		m_is_landing	(false),
		m_collider		(nullptr),
		m_mass_kind		(mass_level_kind)
	{ }
	
	virtual ~PhysicalObjBase() = default;

	virtual void OnCollide(const PhysicalObjBase& check_hit_obj)abstract;

	/// @brief 重力を与える(適用させる)
	/// @brief 物理管理クラスから適用される
	/// @param gravity_acceleration 重力加速度 (デルタタイム適用前)
	/// @param max_gravity 最大重力 (デルタタイム適用後の最大値)
	void ApplyGravity(const float gravity_acceleration, const float max_gravity)
	{
		// 地面にいる場合は重力を与えない
		if (m_is_landing) { return; }

		math::Decrease(m_fall_speed, gravity_acceleration * FPS::GetDeltaTime(), -max_gravity);
		m_fall_velocity.y += m_fall_speed;
	}

	[[nodiscard]] VECTOR	GetFallVelocity()	const { return m_fall_velocity; }
	[[nodiscard]] bool		IsLanding()			const { return m_is_landing; }
	[[nodiscard]] const	std::shared_ptr<ShapeBase> GetCollider() const { return m_collider; }
	[[nodiscard]] const	std::shared_ptr<ShapeBase> GetTrigger(const TriggerKind trigger_kind) const
	{
		if (m_trigger.count(trigger_kind))
		{
			return m_trigger.at(trigger_kind);
		}
		return nullptr;
	}
	[[nodiscard]] MassKind	GetMassKind()		const { return m_mass_kind; }

protected:
	/// @brief コライダーを作成
	/// @brief 上書き不可
	void MakeCollider(std::shared_ptr<ShapeBase> collider)
	{
		if (m_collider != nullptr) { return; }

		m_collider = collider;
	}

	/// @brief トリガーを追加
	/// @brief 異なる種類であれば複数のトリガーの追加を許可
	/// @brief 同種類トリガーの上書き不可
	void AddTrigger(const TriggerKind trigger_kind, std::shared_ptr<ShapeBase> trigger)
	{
		if (!m_trigger.count(trigger_kind))
		{
			m_trigger[trigger_kind] = trigger;
		}
	}

protected:
	float    m_fall_speed;
	VECTOR	 m_fall_velocity;
	VECTOR	 m_velocity;
	bool	 m_is_landing;
	MassKind m_mass_kind;
	std::shared_ptr<ShapeBase> m_collider;
	std::unordered_map<TriggerKind, std::shared_ptr<ShapeBase>> m_trigger;
};
