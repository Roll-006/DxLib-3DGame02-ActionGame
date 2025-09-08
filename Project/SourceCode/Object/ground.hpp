#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Part/modeler.hpp"

#include "../Manager/obj_manager.hpp"
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
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() const { return m_modeler; }

private:
	std::shared_ptr<Modeler> m_modeler;

	static constexpr VECTOR kBasicAngle = { 0.0f, 0.0f, 0.0f };
	static constexpr float  kBasicScale = 5.0f;
	static constexpr VECTOR kPos		= { 0.0f, 0.0f, 0.0f };
<<<<<<< HEAD
=======

	std::shared_ptr<Triangle> t1;
	std::shared_ptr<Triangle> t2;
	std::shared_ptr<Triangle> t3;
	std::shared_ptr<Triangle> t4;
>>>>>>> test/push-back
};
