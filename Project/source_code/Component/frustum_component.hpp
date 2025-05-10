#pragma once
#include "../Base/component_base.hpp"

class FrustumComponent : public ComponentBase
{
public:
	FrustumComponent(GameObj* owner_obj);
	~FrustumComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

private:

};
