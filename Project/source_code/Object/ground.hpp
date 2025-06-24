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

	void OnCollide(const ColliderPairData& hit_collider_pair) override;

private:
	static constexpr float  kModelScale = 600.0f;
	static constexpr VECTOR kPos		= VECTOR(0.0f, 0.0f, 0.0f);
};
