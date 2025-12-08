#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Part/modeler.hpp"
#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class Houses final : public PhysicalObjBase
{
public:
	Houses();
	~Houses() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() const { return m_modeler; }

private:
	std::shared_ptr<Modeler> m_modeler;
	std::shared_ptr<Modeler> m_collision_modeler;

	static constexpr VECTOR kBasicAngle = { 0.0f, 0.0f, 0.0f };
	static constexpr float  kBasicScale = 0.8f;
	static constexpr VECTOR kPos		= { 0.0f, 200.0f, 0.0f };
};
