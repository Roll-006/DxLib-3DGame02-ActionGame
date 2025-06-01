#pragma once
#include "../Base/gun_base.hpp"

class AssaultRifle final : public GunBase
{
public:
	AssaultRifle();
	~AssaultRifle();

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

private:

};
