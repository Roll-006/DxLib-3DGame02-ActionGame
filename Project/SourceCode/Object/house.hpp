#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Part/modeler.hpp"

class House final : public PhysicalObjBase
{
public:
	House();
	~House() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() const { return m_modeler; }

private:
	std::shared_ptr<Modeler> m_modeler;

	static constexpr float  kModelScale = 5.0f;
	static constexpr VECTOR kPos		= { 0.0f, 0.0f, 1000.0f };
};