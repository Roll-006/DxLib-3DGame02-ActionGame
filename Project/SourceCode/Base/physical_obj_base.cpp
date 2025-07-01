#include "physical_obj_base.hpp"

void PhysicalObjBase::ApplyGravity(const float gravity_acceleration, const float max_gravity)
{
	// 地面にいる場合は重力を与えない
	if (m_is_landing)
	{
		m_fall_speed = m_fall_velocity.y = 0.0f;
	}
	else
	{
		math::Decrease(m_fall_speed, gravity_acceleration * FPS::GetDeltaTime(), -max_gravity);
		m_fall_velocity.y += m_fall_speed;
	}
}

void PhysicalObjBase::ApplyVelocity()
{
	m_transform->Move(CoordinateKind::kWorld, m_velocity);

	for (const auto& collider : m_collider)
	{
		const auto shape = collider->GetShape();
		if (shape != nullptr)
		{
			shape->Move(m_velocity);
		}
	}
}

std::shared_ptr<Collider> PhysicalObjBase::GetCollider(const ColliderKind kind) const
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

void PhysicalObjBase::AddCollider(const std::shared_ptr<Collider> collider)
{
	if (std::find(m_collider.begin(), m_collider.end(), collider) == m_collider.end())
	{
		// レイキャストトリガーの場合は線分以外許可しない
		if (collider->GetColliderKind() == ColliderKind::kRayCast)
		{
			assert(collider->GetShape()->GetShapeKind() == ShapeKind::kSegment);
		}

		m_collider.emplace_back(collider);
	}
}
