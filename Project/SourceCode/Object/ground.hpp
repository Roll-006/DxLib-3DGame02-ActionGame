#pragma once
#include "../Base/physical_obj_base.hpp"

class Ground final : public PhysicalObjBase
{
public:
	Ground();
	~Ground() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() { return m_modeler; }

private:
	std::shared_ptr<Modeler> m_modeler;

	static constexpr float  kModelScale = 600.0f;
	static constexpr VECTOR kPos		= { 0.0f, 0.0f, 0.0f };
};
