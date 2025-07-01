#pragma once
#include "../Data/Kind/collider_kind.hpp"
#include "../Data/IncludeList/shape.hpp"
//#include "../Base/physical_obj_base.hpp"

class PhysicalObjBase;

class Collider final
{
public:
	Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase> shape, PhysicalObjBase* owner_obj) :
		m_kind					(kind),
		m_shape					(shape),
		m_model_handle			(-1),
		m_is_closest_only_hit	(kind == ColliderKind::kRayCast ? true : false),
		m_owner_obj				(owner_obj)
	{ }

	Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase> shape, const bool is_closest_only_hit, PhysicalObjBase* owner_obj) :
		m_kind					(kind),
		m_shape					(shape),
		m_model_handle			(-1),
		m_is_closest_only_hit	(kind == ColliderKind::kRayCast ? is_closest_only_hit : false),
		m_owner_obj				(owner_obj)
	{ }

	Collider(const ColliderKind kind, const int model_handle, PhysicalObjBase* owner_obj) :
		m_kind					(kind),
		m_shape					(nullptr),
		m_model_handle			(model_handle),
		m_is_closest_only_hit	(false),
		m_owner_obj				(owner_obj)
	{ }

	~Collider() { m_owner_obj = nullptr; }

	/// @brief すべてのコライダーとの衝突判定を有効にする(レイキャストでのみ有効な関数)
	void EnableAllRayCastHit()
	{
		if (m_kind == ColliderKind::kRayCast) { m_is_closest_only_hit = false; }
	}
	/// @brief 最も近いコライダーとの衝突判定のみを有効にする(レイキャストでのみ有効な関数)
	void EnableClosestOnlyRayCastHit()
	{
		if (m_kind == ColliderKind::kRayCast) { m_is_closest_only_hit = true; }
	}

	[[nodiscard]] ColliderKind GetColliderKind()		const { return m_kind; }
	[[nodiscard]] std::shared_ptr<ShapeBase> GetShape() const { return m_shape; }
	[[nodiscard]] int GetModelHandle()					const { return m_model_handle; }
	[[nodiscard]] PhysicalObjBase* GetOwnerObj()		const { return m_owner_obj; }

	/// @brief 最も近いコライダーとのみ衝突を許可するかを判定(レイキャストでのみ有効な関数)
	[[nodiscard]] bool IsClosestOnlyHit() const { return m_is_closest_only_hit; }

private:
	ColliderKind m_kind;
	std::shared_ptr<ShapeBase> m_shape;
	int		m_model_handle;
	bool	m_is_closest_only_hit;			// 自身と最も近いコライダーのみ衝突を許可する(レイキャストでのみ有効)
	PhysicalObjBase* m_owner_obj;
};
