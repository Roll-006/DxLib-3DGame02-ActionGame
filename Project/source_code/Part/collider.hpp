#pragma once
#include "../Data/Kind/collider_kind.hpp"
#include "../Data/IncludeList/shape.hpp"
//#include "../Base/physical_obj_base.hpp"

class PhysicalObjBase;

class Collider final
{
public:
	Collider(const ColliderKind kind, const std::shared_ptr<ShapeBase> shape, PhysicalObjBase* owner_obj) :
		m_kind			(kind),
		m_shape			(shape),
		m_model_handle	(-1),
		m_owner_obj		(owner_obj)
	{ }

	Collider(const ColliderKind kind, const int model_handle, PhysicalObjBase* owner_obj) :
		m_kind			(kind),
		m_shape			(nullptr),
		m_model_handle	(model_handle),
		m_owner_obj		(owner_obj)
	{ }

	~Collider() { m_owner_obj = nullptr; }

	[[nodiscard]] ColliderKind GetColliderKind()		const { return m_kind; }
	[[nodiscard]] std::shared_ptr<ShapeBase> GetShape() const { return m_shape; }
	[[nodiscard]] int GetModelHandle()					const { return m_model_handle; }
	[[nodiscard]] PhysicalObjBase* GetOwnerObj()		const { return m_owner_obj; }

private:
	ColliderKind m_kind;
	std::shared_ptr<ShapeBase> m_shape;
	int m_model_handle;
	PhysicalObjBase* m_owner_obj;
};
