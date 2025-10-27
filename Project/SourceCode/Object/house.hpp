#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Part/modeler.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class House final : public PhysicalObjBase
{
public:
	House();
	~House() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() const { return m_modeler; }

private:
	std::shared_ptr<Modeler> m_modeler;

	static constexpr VECTOR kBasicAngle				= { 0.0f, 0.0f, 0.0f };
	static constexpr float  kBasicScale				= 0.27f;
	static constexpr VECTOR kPos					= { 615.0f, -90.0f, 372.0f };
	static constexpr VECTOR kCollisionAreaOffset	= { 0.0f, 150.0f, 0.0f };
	static constexpr float  kCollisionAreaRadius	= 250.0f;
};
