#pragma once
#include "../Base/obj_base.hpp"

#include "../Data/Kind/mass_kind.hpp"
#include "../Part/collider.hpp"
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
		//if (m_is_landing) { return; }

		math::Decrease(m_fall_speed, gravity_acceleration * FPS::GetDeltaTime(), -max_gravity);
		m_fall_velocity.y += m_fall_speed;
	}

	[[nodiscard]] VECTOR	GetVelocity()		const { return m_velocity; }
	[[nodiscard]] VECTOR	GetFallVelocity()	const { return m_fall_velocity; }
	[[nodiscard]] MassKind	GetMassKind()		const { return m_mass_kind; }
	[[nodiscard]] std::shared_ptr<Collider> GetCollider(const ColliderKind kind)
	{
		for (const auto& collider : m_collider)
		{
			if (collider->GetColliderKind() == kind)
			{
				return collider;
			}
		}
		return nullptr;
	}
	[[nodiscard]] std::vector<std::shared_ptr<Collider>> GetColliderAll() const { return m_collider; }

protected:
	/// @brief コライダーを追加
	/// @brief 異なる種類であれば複数のコライダーの追加を許可
	/// @brief 同種類コライダーであっても図形が異なれば上書きを許可
	void AddCollider(const std::shared_ptr<Collider> collider)
	{
		if (std::find(m_collider.begin(), m_collider.end(), collider) == m_collider.end())
		{
			m_collider.emplace_back(collider);
		}
	}

	/// @brief 速度ベクトルをコライダーに適用
	void ApplyVelocityToCollider()
	{
		for (const auto& collider : m_collider)
		{
			collider->GetShape()->Move(m_velocity);
		}
	}

protected:
	float    m_fall_speed;
	VECTOR	 m_fall_velocity;
	MassKind m_mass_kind;
	std::vector<std::shared_ptr<Collider>> m_collider;

public:
	//TODO : 仮でpublicに移行。のちに変更
	VECTOR	 m_velocity;
};
