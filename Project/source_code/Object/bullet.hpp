#pragma once
#include "../Base/physical_obj_base.hpp"

class Bullet final : public PhysicalObjBase
{
public:
	Bullet();
	~Bullet() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairData& hit_collider_pair) override;
};