#pragma once
#include "../Base/physical_obj_base.hpp"

class ShellCasing final : public PhysicalObjBase
{
public:
	ShellCasing();
	~ShellCasing() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

private:

};
