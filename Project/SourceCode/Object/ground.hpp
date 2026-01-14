#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Part/modeler.hpp"

#include "../Accessor/obj_accessor.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class Ground final : public PhysicalObjBase
{
public:
	Ground();
	~Ground() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
	void OnProjectPos() override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	[[nodiscard]] const float GetDeltaTime() const override;
	[[nodiscard]] const std::shared_ptr<const Modeler> GetModeler() const { return m_modeler; }

private:
	std::shared_ptr<Modeler> m_modeler;

	static constexpr VECTOR kBasicAngle = { 0.0f, 0.0f, 0.0f };
	static constexpr float  kBasicScale = 0.8f;
	static constexpr VECTOR kPos		= { 0.0f, 200.0f, 0.0f };
};
