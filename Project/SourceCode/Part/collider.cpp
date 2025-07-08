#include "collider.hpp"

Collider::Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase> shape, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(shape),
	m_model_handle			(-1),
	m_is_closest_only_hit	(kind == ColliderKind::kRayCast ? true : false),
	m_owner_obj				(owner_obj)
{
	// 処理なし
}

Collider::Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase> shape, const bool is_closest_only_hit, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(shape),
	m_model_handle			(-1),
	m_is_closest_only_hit	(kind == ColliderKind::kRayCast ? is_closest_only_hit : false),
	m_owner_obj				(owner_obj)
{
	// 処理なし
}

Collider::Collider(const ColliderKind kind, const int model_handle, PhysicalObjBase* owner_obj) :
	m_kind					(kind),
	m_shape					(nullptr),
	m_model_handle			(model_handle),
	m_is_closest_only_hit	(false),
	m_owner_obj				(owner_obj)
{
	// 処理なし
}